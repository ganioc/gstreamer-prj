// 创建一个nalu解析器
GstH264NalParser *nalu_parser = gst_h264_nal_parser_new();




GstMapInfo map_info;
bool b = gst_buffer_map(buffer, &map_info, GST_MAP_READ); // buffer 是GstBuffer类型
if(!b)
{
    return;
}

GstH264NalUnit nalu;
GstH264ParserResult parse_result = gst_h264_parser_identify_nalu(parser, map_info.data, 0, map_info.size, &nalu);

if (parse_result == GST_H264_PARSER_NO_NAL_END)
{
    if (nalu.type == GST_H264_NAL_SLICE || nalu.type == GST_H264_NAL_SLICE_IDR) // 在I帧或P帧前面插SEI,也可以在其他位置插入，根据nalu.type判断
    {
        nlohmann::json json_object = nlohmann::json::object(); // 这里以json举例，可以插入任意序列化数据，设置data ptr 和 data size就行了
        json_object["index"] = 0;
        std::string json_data = json_object.dump();
        GArray *message = g_array_new(false, false, sizeof(GstH264SEIMessage));

        GstH264RegisteredUserData register_user_data{
                                0, 0,
                                (const guint8 *)json_data.data(),
                                json_data.size()};
        GstH264SEIMessage sei_msg;
        sei_msg.payloadType = GST_H264_SEI_REGISTERED_USER_DATA;
        sei_msg.payload.registered_user_data = register_user_data;
        g_array_append_val(message, sei_msg);

        GstMemory *mem = gst_h264_create_sei_memory(4, message);
        GstBuffer *buffer_sei = gst_h264_parser_insert_sei(parser, buffer, mem);
        gst_buffer_unref(buffer_sei);  // 根据情况选择push到下个pad或者unref
        gst_memory_unref(mem);
        g_array_unref(message);
    }
}



// 释放nalu解析器
gst_h264_nal_parser_free(nalu_parser);