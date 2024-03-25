#pragma once
#include <string>
#include <gst/gst.h>

class CameraStreamer {
public:
    CameraStreamer();
    ~CameraStreamer();
    void StartStreaming();

    static const std::string kOutputDirectory;
    static const std::string kFileName;

private:
    GstElement *pipeline, *source, *sink, *parser, *encoder;
};