#ifndef QPARSE_H
#define QPARSE_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QPair>
#include <QStringList>
#include <QMimeDatabase>
#include <QDateTime>
#include <QUrl>
#include <QQueue>
#include <QJsonObject>
#include <QJsonValue>
#include <QVariantMap>
#include <QTimer>
#include <QNetworkDiskCache>
#include <QQmlNetworkAccessManagerFactory>

class QNetworkAccessManager;
class QNetworkRequest;
class QNetworkReply;
class OperationData;
class QFile;
class QParseRequest;
class QParseReply;
class QParseObject;
class QParseUser;
class QParseDiskCache;
class QParseNetworkAccessManagerFactory;

/*! This class creates allow to connect to PARSE cloud and
 *  perform operations on it as query, storing and retriveing data
 *
 *  This object is a singleton
 *
 *  Now, it use REST API of PARSE
 */
class QParse : public QObject {
	Q_OBJECT
	//! PARSE Application ID
	Q_PROPERTY( QString appId MEMBER appId NOTIFY appIdChanged )
	//! PARSE REST API Key (use the client key, not the master)
	Q_PROPERTY( QString restKey MEMBER restKey NOTIFY restKeyChanged )
	//! this allow to bind the logged user to QML properties
	Q_PROPERTY( QParseUser* me READ getMe NOTIFY meChanged )
public:
	//! return the singleton instance of CloudInterface
	static QParse* instance();
public slots:
	QString getAppId() const;
	void setAppId(const QString &value);
	QString getRestKey() const;
	void setRestKey(const QString &value);
	/*! return the Json value of the specified PARSE config
	 *  \note before access to any app config, make sure you downloaded the app config
	 *		  with updateAppConfigValues
	 */
	QJsonValue getAppConfigValue( QString key );
	//! update the app config values
	void updateAppConfigValues();
	//! return the logged user; a NULL pointer means no user is logged in
	QParseUser* getMe();
	//! Perform a get request on PARSE
	QParseReply* get( QParseRequest* request );
	//! Perform a post request on PARSE
	QParseReply* post( QParseRequest* request );
	//! Perform a put request on PARSE
	QParseReply* put( QParseRequest* request );
signals:
	void appIdChanged( QString appId );
	void restKeyChanged( QString restKey );
	//! emitted when the app config has been updated (retrieve them using getAppConfigValue
	void appConfigChanged();
	void meChanged( QParseUser* user );
private slots:
	//! it manage the returned data from the cloud backed
	void onRequestFinished( QNetworkReply* reply );
private:
	// private constructor; this is a singleton
	QParse(QObject *parent = 0);
	Q_DISABLE_COPY( QParse )

	/*! process a queued QParseRequest and create the corresponding
	 *  QNetworkRequest to send over internet for the reply to PARSE
	 */
	void processOperationsQueue();

	//! PARSE Keys
	QString appId;
	QString restKey;

	//! The App config
	QJsonObject appConfig;

	//! current user
	QParseUser* user;

	//! the newtork manager for sending requests to cloud backend
	QNetworkAccessManager* net;
	//! inner private class for storing data about operations on Parse
	class OperationData {
	public:
		OperationData()
			: parseRequest(NULL)
			, parseReply(NULL)
			, netRequest(NULL)
			, netMethod(GET)
			, dataToPost()
			, fileToPost(NULL)
			, mimeDb() { }
		QParseRequest* parseRequest;
		QParseReply* parseReply;
		QNetworkRequest* netRequest;
		enum NetMethod { GET, PUT, POST, DELETE };
		NetMethod netMethod;
		QJsonObject dataToPost;
		QFile* fileToPost;
		QMimeDatabase mimeDb;
	};
	/*! the queue of the operation to process
	 *  A parseRequest will be put in this queue and as soon as possible
	 *  will be processed and created a netRequest to send over internet
	 */
	QQueue<OperationData*> operationsQueue;
	//! the map of operation sent to Parse waiting for a netReply to process
	QMap<QNetworkReply*, OperationData*> operationsPending;

	//! Timer for triggering the execution of processOperationsQueue()
	QTimer* timer;

	//! inner private class for handling cached items
	class CacheData {
	public:
		//! the cached data can be only a Json object or a binary file
		bool isJson;
		//! the local file where data is cached
		QUrl localFile;
		//! date of creation
		QDateTime createdAt;
	};
	//! all cached data indexed by QUrl request
	QMap<QUrl, CacheData> cache;
	//! writable cache directory
	QString cacheDir;
	//! INI file containing the cache data info
	QString cacheIni;
	//! load all cache data info from the disk
	void loadCacheInfoData();
	//! update/write a cache element
	void updateCache( QNetworkReply* reply, OperationData* opdata );
	//! return true if there is a valid cached data for given request
	bool isRequestCached( QUrl url );
	//! fill the reply with cached data
	void fillWithCachedData( QUrl url, QParseReply* reply );
	//! return the Json object cached at given url
	QJsonObject getCachedJson( QUrl url );

	//! return a unique file name into the cacheDir (return full path)
	QString getUniqueCacheFilename();
};

#endif // QPARSE_H
