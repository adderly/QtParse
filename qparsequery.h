#ifndef QPARSEQUERY_H
#define QPARSEQUERY_H

#include <QObject>
#include "qparse.h"
#include "qparseobject.h"
#include "qparsetypes.h"

/*! Instanced an object of this class for perform a query on PARSE
 *  and retrieving a collection of PARSE objects
 */
class QParseQuery : public QObject {
	Q_OBJECT
	//! the cache control
	Q_PROPERTY( QParse::CacheControl cacheControl MEMBER cacheControl )
public:
	/*! constructor a new query */
	template<class ParseObject>
	static QParseQuery* create( QString className=QString() ) {
		if ( className.isEmpty() ) {
			ParseObject p;
			className = p.parseClassName();
		}
		return new QParseQuery(className, ParseObject::staticMetaObject);
	}	
public slots:
	//! create a where clause
	QParseQuery* whereIn( QString property, QStringList values );
	//! specify how to order
	QParseQuery* orderBy( QString property, bool descending=false );
	//! execute the query
	void query();

	QParse::CacheControl getCacheControl() const;
	void setCacheControl(const QParse::CacheControl &value);
signals:
	//! return the all retrieved objects
	void queryResults( QList<QParseObject*> results );
	//! emitted when there is some error
	void queryError( QString message );
protected:
private slots:
	/*! handle the completion of get request on PARSE */
	void onQueryReply( QParseReply* reply );
private:
	// disable public constructors
	QParseQuery( QString parseClassName, QMetaObject metaParseObject );
	Q_DISABLE_COPY( QParseQuery )

	//! the cache control to use
	QParse::CacheControl cacheControl;

	//! the underlying QParseRequest to use
	QParseRequest* queryRequest;

	//! the JsonObject containing the Where clause
	QJsonObject where;

	//! the QMetaObject used for constructing the right QParseObject
	QMetaObject metaParseObject;
	//! the Parse class name target of the query
	QString parseClassName;
};

#endif // QPARSEQUERY_H
