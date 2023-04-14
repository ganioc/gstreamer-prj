// 创建一个nalu解析器
GstH264NalParser *nalu_parser = gst_h264_nal_parser_new();

GstH264NalUnit nalu;
GstH264ParserResult parse_result = gst_h264_parser_identify_nalu(parser, map_info.data, 0, map_info.size, &nalu);

if (parse_result == GST_H264_PARSER_NO_NAL_END)
{
    if (nalu.type == GST_H264_NAL_SEI)
    {
         GArray *array = g_array_new(false, false, sizeof(GstH264SEIMessage));
         gst_h264_parser_parse_sei(parser, &nalu, &array);
         GstH264SEIMessage sei_msg = g_array_index(array,GstH264SEIMessage, 0);
         GstH264RegisteredUserData register_user_data = sei_msg.payload.registered_user_data;
         std::string str((const char*)register_user_data.data, register_user_data.size);
         nlohmann::json json_object = nlohmann::json::parse(str);
         std::cout << json_object << std::endl;
         g_array_free(array, true);
     }
}

// 释放nalu解析器
gst_h264_nal_parser_free(nalu_parser);

