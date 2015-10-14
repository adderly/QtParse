#ifndef QPARSETYPES_H
#define QPARSETYPES_H

#include <QJsonObject>
#include <QDateTime>
#include <QUrl>
#include <QMetaType>

//! \file This file contains the classes for handle easily the PARSE data types

//! The Date type on PARSE
class QParseDate {
public:
	//! default constructor
	QParseDate();
	//! copy-constructor
	QParseDate( const QParseDate& src );
	//! construct a QParseDate from the Json PARSE representation
	QParseDate( QJsonObject fromParse );
	//! construct a QParseDate from a QDateTime object
	QParseDate( QDateTime fromDateTime );
	bool operator==( QParseDate const& other ) const;
	bool operator!=( QParseDate const& other ) const;
	//! export into Json PARSE representation
	QJsonObject toJson() const;
	//! export into QDateTime
	QDateTime toDateTime() const;
private:
	//! Json representation for PARSE
	QJsonObject json;
	//! QDateTime representation
	QDateTime dateTime;
};
Q_DECLARE_METATYPE( QParseDate )

//! The File type on PARSE
class QParseFile {
public:
	//! default constructor
	QParseFile();
	//! copy-constructor
	QParseFile( const QParseFile& src );
	//! construct a QParseFile from the Json PARSE representation
	QParseFile( QJsonObject fromParse );
	//! construct a QParseFile from a local file
	QParseFile( QUrl localFile );
	bool operator==( QParseFile const& other ) const;
	bool operator!=( QParseFile const& other ) const;
	/*! the URL where the file is currently stored
	 *
	 *  If the URL points to a local file it means
	 *  is not uploaded to PARSE yet
	 *
	 *  If the URL is remote, then it means the file
	 *  has been uploaded to PARSE
	 */
	QUrl getUrl() const;
	/*! the name of the file on PARSE
	 *
	 *  If the file has not been uploaded to PARSE yet,
	 *  this name will be empty
	 */
	QString getName() const;
	//! export into Json PARSE representation
	QJsonObject toJson();
private:
	//! Json representation for PARSE
	QJsonObject json;
	/*! the URL where the file is currently stored
	 *
	 *  If the URL points to a local file it means
	 *  is not uploaded to PARSE yet
	 *
	 *  If the URL is remote, then it means the file
	 *  has been uploaded to PARSE
	 */
	QUrl url;
	/*! the name of the file on PARSE
	 *
	 *  If the file has not been uploaded to PARSE yet,
	 *  this name will be empty
	 */
	QString name;
};
Q_DECLARE_METATYPE( QParseFile )

#endif // QPARSETYPES_H
