#include "camera_streamer.h"

#include <iostream>
#include <chrono>
#include <filesystem>

namespace fs = std::filesystem;

const std::string CameraStreamer::kOutputDirectory = "C:/GStreamerDemo/output/";

// Constructor
CameraStreamer::CameraStreamer() {
  // Creates directory if doesn't already exist.
  fs::create_directory(kOutputDirectory);
  
  // Gets the current date and time as a string.
  // Used for timestamping videos with the program's starting datetime to avoid overwriting older videos.
  auto const CurrentTime = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
  std::string ProgramStartTime = std::format("{:%Y_%m_%d_%H_%M_%S}", CurrentTime);
  
  gst_init(nullptr, nullptr);

  // Create the elements of the pipeline. An encoder and parser are required to
  // export the video files into a usable format.
  pipeline = gst_pipeline_new("webcam-streaming-pipeline");
  source = gst_element_factory_make("mfvideosrc", "video_source");
  encoder = gst_element_factory_make("x264enc", "video_encoder");
  parser = gst_element_factory_make("h264parse", "parser");
  sink = gst_element_factory_make("splitmuxsink", "video_sink");

  // Error handling
  if (!pipeline || !source || !parser || !sink) {
    std::cerr << "Failed to create pipeline and elements. Please ensure "
                 "GStreamer has been installed correctly and you are running "
                 "on the correct platform."
              << std::endl;
    return;
  }
  if (!encoder) {
    std::cerr << "Failed to create MP4 handling elements. Please ensure all "
                 "GStreamer plugins have been installed correctly."
              << std::endl;
    return;
  }

  // Defines the key-int-max value as some arbitrarily small value. Without this
  // video lengths can deviate up to 15s as it attempts to end on the encoder's
  // key frames.
  g_object_set(G_OBJECT(encoder), "key-int-max", 10, NULL);

  // Configures the multifilesink's output location and maximum video length.
  g_object_set(G_OBJECT(sink), 
      "location", (kOutputDirectory + ProgramStartTime + "_%05d.mp4").c_str(),
      "max-size-time", (guint64)(60 * GST_SECOND), 
      NULL);

  // Builds the pipeline with the gst elements.
  gst_bin_add_many(GST_BIN(pipeline), source, encoder, parser, sink, NULL);
  if (!gst_element_link_many(source, encoder, parser, sink, NULL)) {
    std::cerr << "Failed to link source to encoder." << std::endl;
    gst_object_unref(pipeline);
    return;
  }
}

// Destructor
CameraStreamer::~CameraStreamer() {
  gst_element_set_state(pipeline, GST_STATE_NULL);
  gst_object_unref(pipeline);
}

// The primary function of the class, handles running and closing the pipeline.
void CameraStreamer::StartStreaming() {
  // Main loop
  gst_element_set_state(pipeline, GST_STATE_PLAYING);
  GMainLoop* loop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(loop);

  // Clean up
  gst_element_set_state(pipeline, GST_STATE_NULL);
  gst_object_unref(GST_OBJECT(pipeline));
  g_main_loop_unref(loop);
}
