#include "voiceplayer.h"
#include "ui_voiceplayer.h"
#include "msgdef.h"

VoicePlayer::VoicePlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VoicePlayer)
{
    ui->setupUi(this);
}

VoicePlayer::~VoicePlayer()
{
    delete ui;
}

void VoicePlayer::setMessage(const Message &msg)
{
//    m_message = msg;
//    for(int i=0; i<msg.items().count(); i++)
//    {
//        QString emotion;
//        bool    ok = false;
//        int     emotionIndex = 0;
//        QString localFileName;
//        QString imageFile;
//        QFile   file;
//        auto item = msg.items().at(i);

//        switch(item.type)
//        {
//        case BasicDef::MIT_VOICE:
//            break;

//        case BasicDef::MIT_IMAGE:
//            localFileName = item.data;
//            if(localFileName.contains("/"))
//            {
//                localFileName = localFileName.mid(localFileName.lastIndexOf("/") + 1);
//            }
//            imageFile = localFileName;//m_chatImagePath + "/" + localFileName;
//            file.setFileName(imageFile);
//            if(file.exists())
//            {
//                appendImage(QUrl::fromLocalFile(imageFile).toString());
//            }

//            break;

//        case BasicDef::MIT_EMOTICONS:
//            emotion = item.data;
//            if (emotion.startsWith("face"))
//            {
//                emotion.remove("face");
//            }
//            emotionIndex = emotion.toInt(&ok);
//            if (ok)
//            {
//                appendBaseFace(emotionIndex);
//            }
//            break;

//        case BasicDef::MIT_GIF:
//            emotion = item.data;
//            if (emotion.startsWith("bface"))
//            {
//                emotion.remove("bface");
//            }
//            ok = false;
//            emotionIndex = emotion.toInt(&ok);
//            if (ok)
//            {
//                appendHuhooFace(emotionIndex);
//            }
//            break;

//        default:
//            break;
//        }
//    }

    update();
    updateGeometry();
}

Message VoicePlayer::message() const
{
    return m_message;
}
