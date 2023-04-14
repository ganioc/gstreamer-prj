static void handoff_callback(GstElement *identity, GstBuffer *buffer,
                             gpointer user_data) {
  GST_TRACE("handoff_callback");
  GstMapInfo info = GST_MAP_INFO_INIT;
  GstH264NalParser *nalparser = NULL;
  GstH264NalUnit nalu;

  if (gst_buffer_map(buffer, &info, GST_MAP_READ)) {
    nalparser = gst_h264_nal_parser_new();
    if (NULL != nalparser) {
      if (GST_H264_PARSER_OK ==
          gst_h264_parser_identify_nalu_unchecked(nalparser, info.data, 0,
                                                  info.size, &nalu)) {
        // if (info.size < 100) GST_LOG("buffer info size %ld", info.size);
        if (GST_H264_NAL_SEI == nalu.type) {
          GST_LOG(
              "identify sei nalu with size = %d, offset = %d, sc_offset = %d",
              nalu.size, nalu.offset, nalu.sc_offset);
          int64_t delay = -1;
          if (TRUE ==
              h264_sei_ntp_parse(nalu.data + nalu.offset, nalu.size, &delay)) {
            GST_LOG("delay = %ld ms", delay);
          }
        }
      } else {
        GST_WARNING("gst_h264_parser_identify_nalu_unchecked failed");
      }

      gst_h264_nal_parser_free(nalparser);
    } else {
      GST_WARNING("gst_h264_nal_parser_new failed");
    }

    gst_buffer_unmap(buffer, &info);
  } else {
    GST_WARNING("gst_buffer_map failed");
  }
}
