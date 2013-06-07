/* Webcamod, webcam capture plasmoid.
 * Copyright (C) 2011-2013  Gonzalo Exequiel Pedone
 *
 * Webcamod is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Webcamod is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Webcamod. If not, see <http://www.gnu.org/licenses/>.
 *
 * Email     : hipersayan DOT x AT gmail DOT com
 * Web-Site 1: http://github.com/hipersayanX/Webcamoid
 * Web-Site 2: http://kde-apps.org/content/show.php/Webcamoid?content=144796
 */

#ifndef MULTISINKELEMENT_H
#define MULTISINKELEMENT_H

#include <qb.h>

extern "C"
{
    #include <libavformat/avformat.h>
    #include <libavutil/imgutils.h>
}

#include "commands.h"
#include "outputparams.h"

typedef QMap<QString, QString> StringMap;

class MultiSinkElement: public QbElement
{
    Q_OBJECT

    Q_PROPERTY(QString location READ location
                                WRITE setLocation
                                RESET resetLocation)

    Q_PROPERTY(QString options READ options
                               WRITE setOptions
                               RESET resetOptions)

    Q_PROPERTY(StringMap streamCaps READ streamCaps
                                    WRITE setStreamCaps
                                    RESET resetStreamCaps)

    public:
        explicit MultiSinkElement();
        ~MultiSinkElement();

        Q_INVOKABLE QString location();
        Q_INVOKABLE QString options();
        Q_INVOKABLE StringMap streamCaps();

    protected:
        bool init();
        void uninit();

    private:
        QString m_location;
        QString m_options;
        StringMap m_streamCaps;
        QSize m_frameSize;

        AVFormatContext *m_outputContext;
        Commands m_commands;
        QVariantMap m_optionsMap;
        AVStream *m_audioStream;
        AVStream *m_videoStream;
        QbElementPtr m_filter;
        QbCaps m_curAInputCaps;
        QbCaps m_curVInputCaps;
        QMap<QString, OutputParams> m_outputParams;

        OutputParams createOutputParams(const QbCaps &inputCaps,
                                        const QVariantMap &options);

        QList<PixelFormat> pixelFormats(AVCodec *videoCodec);
        QList<AVSampleFormat> sampleFormats(AVCodec *audioCodec);
        QList<int> sampleRates(AVCodec *audioCodec);
        QList<uint64_t> channelLayouts(AVCodec *audioCodec);
        AVStream *addStream(AVCodec **codec, QString codecName="", AVMediaType mediaType=AVMEDIA_TYPE_UNKNOWN);

    public slots:
        void setLocation(QString location);
        void setOptions(QString options);
        void setStreamCaps(StringMap streamCaps);
        void resetLocation();
        void resetOptions();
        void resetStreamCaps();

        void iStream(const QbPacket &packet);
        void setState(ElementState state);

    private slots:
        void processVFrame(const QbPacket &packet);
        void processAFrame(const QbPacket &packet);
        void updateOutputParams();
};

#endif // MULTISINKELEMENT_H
