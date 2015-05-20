#ifndef VOICEPLAYER_H
#define VOICEPLAYER_H

#include <QWidget>
#include "message.h"
#include <QtMultimedia/QMediaPlayer>

namespace Ui {
class VoicePlayer;
}

class VoicePlayer : public QWidget
{
    Q_OBJECT

public:
    explicit VoicePlayer(QWidget *parent = 0);
    ~VoicePlayer();

    void setMessage(const Message& msg);
    Message message() const;

private:
    //std::function<const QString(const QString&)> m_funcVoiceLocalUrl;
    //std::function<void(const QString &)> m_funcGetVoiceFile;
    QMediaPlayer *m_msgVoicePlayer;
    QMap<QString, QString> m_msgVoiceMap; // <localVoiceUrl, VoiceIconFilePath>
    QString m_inPlayVoiceImgPath;
    QString m_inReadyVoiceImgPath;
    QString m_outPlayVoiceImgPath;
    QString m_outReadyVoiceImgPath;
    QString m_currentLocalVoiceUrl;
    //void refreshVoiceImage(QString localVoiceUrl, bool play);
    Message m_message;
private slots:
    //void stopPlaying(QMediaPlayer::MediaStatus status);


private:
    Ui::VoicePlayer *ui;
};

#endif // VOICEPLAYER_H
