#include <QCoreApplication>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QUrl url("http://192.168.205.131:5000/math");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("User-Agent", "MyApp");

    QJsonObject jsonData;
    jsonData["num1"] = "15";
    jsonData["num2"] = "18";
    QJsonDocument jsonDoc(jsonData);
    QByteArray requestData = jsonDoc.toJson();

    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.post(request, requestData);

    QObject::connect(reply, &QNetworkReply::finished, [&]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
            QJsonObject jsonObj = jsonDoc.object();
            qDebug() << jsonObj;

            QString name = jsonObj.value("sum").toString();
            QString code = jsonObj.value("multiply").toString();

            qDebug() << "sum:" << name;
            qDebug() << "multiply:" << code;
        } else {
            qDebug() << "Error:" << reply->errorString();
            QByteArray errorData = reply->readAll();
            QJsonDocument errJsonDoc = QJsonDocument::fromJson(errorData);
            QJsonObject errJsonObj = errJsonDoc.object();
            qDebug() << "Error:" << errJsonObj.value("error").toString();
        }

        reply->deleteLater();
        a.quit();
    });

    return a.exec();
}
