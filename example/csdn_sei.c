static void need_data_callback(GstElement *appsrc, guint unused,
                               gpointer udata) {
  GST_LOG("need_data_callback");
  GstBuffer *buffer;
  GstFlowReturn ret;
  static uint64_t next_ms_time_insert_sei = 0;
  struct timespec one_ms;
  struct timespec rem;
  uint8_t *h264_sei = NULL;
  size_t length = 0;

  one_ms.tv_sec = 0;
  one_ms.tv_nsec = 1000000;

  while (now_ms() <= next_ms_time_insert_sei) {
    GST_TRACE("sleep to wait time trigger");
    nanosleep(&one_ms, &rem);
  }

  if (!h264_sei_ntp_new(&h264_sei, &length)) {
    GST_ERROR("h264_sei_ntp_new failed");
    return;
  }

  if (NULL != h264_sei && length > 0) {
    buffer =
        gst_buffer_new_allocate(NULL, START_CODE_PREFIX_BYTES + length, NULL);
    if (NULL != buffer) {
      // fill start_code_prefix: 0x00000001
      uint8_t start_code_prefix[] = START_CODE_PREFIX;
      gst_buffer_fill(buffer, 0, start_code_prefix, START_CODE_PREFIX_BYTES);
      // fill H.264 SEI
      size_t bytes_copied =
          gst_buffer_fill(buffer, START_CODE_PREFIX_BYTES, h264_sei, length);
      if (bytes_copied == length) {
        g_signal_emit_by_name(appsrc, "push-buffer", buffer, &ret);
        GST_DEBUG("H264 SEI NTP timestamp inserted");
      } else {
        GST_ERROR("GstBuffer.fill without all bytes copied");
      }
    } else {
      GST_ERROR("GstBuffer.new_allocate failed");
    }
    gst_buffer_unref(buffer);
  }

  next_ms_time_insert_sei = now_ms() + 1000;
  free(h264_sei);
}
