/***************************************************************
 * Name:      video.cpp
 * Purpose:   Code for grabbing the video
 * Author:    Chad Rempp (crempp@gmail.com)
 * Created:   2008-10-28
 * Copyright: Chad Rempp (http://www.tetractys.org)
 * License:
 **************************************************************/

#include <stdio.h>
#include <unicap.h>
//#include <wx/wx.h>
#include "mainframe.h"
#include "video.h"

extern unicap_device_t device_list[4];
extern unicap_handle_t handle;

camDevice::camDevice( string id )
{
	unicap_device_t spec;
	unicap_void_device( &spec );
	strcpy( spec.identifier, id.c_str() );

	unicap_enumerate_devices( &spec, &m_device, 0 );

	m_handle = NULL;
}


/*
unicap_data_buffer_t buffer;
int frame_count;

void list_devices(void){
	int status = STATUS_SUCCESS;
    wxString temp;

    wxLogMessage(_T("  Listing devices"));
    for ( int i = 0; SUCCESS(status) && (i < MAX_DEVICES); i++){
        status = unicap_enumerate_devices( NULL, &device_list[i], i);
        if (SUCCESS(status)){
            temp = wxString::FromAscii(device_list[i].identifier);
            wxLogMessage(_T("  %d: %s"),i,temp.c_str());
        }
        else{
            wxLogError(_T("Couldn't enumerate device %d"), i);
            break;
        }
	}
}

void load_device(int dev_num){
    //unicap_format_t format_spec;
    //unicap_void_format(&format_spec);
    unicap_format_t format;
    int status_1;
    int status_2;
    int status_3;
    int status_4;
    int status_5;
    int status_6;
    wxString temp;

    wxLogMessage(_T("  Loading device %d"), dev_num);

    // Open the selected device
    status_1 = unicap_open(&handle, &device_list[dev_num]);
    if (!SUCCESS(status_1)){
        wxLogError(_T("Couldn't open device, %x"), status_1);
        exit(-1);
    }
    wxLogMessage(_T("  Opened device, %x"), status_1);

    // Set buffer type to use system buffers

    status_2 = unicap_get_format( handle, &format );
    if( !SUCCESS(status_2) ){
        wxLogError(_T("Couldn't get format, %x"), status_2);
        exit(-1);
    }
    temp = wxString::FromAscii(format.identifier);
    wxLogMessage(_T("  id = %s"), temp);
    //wxLogMessage(_T("  id = %s, bs = %d, w = %d, h = %d"), temp, format.buffer_size, format.size.width, format.size.height);

    /* THIS ISN'T WORKING -- WHY?
    format.buffer_type = UNICAP_BUFFER_TYPE_USER;
    format.size.width  = 640;
    format.size.height = 480;
    status_3 = unicap_set_format( handle, &format );
    if( !SUCCESS(status_3))
    {
       wxLogError(_T("Couldn't set format, %x"), status_3);
    //   exit(-1);
    }
    */

    // Create buffer
    buffer.data = (unsigned char*)malloc(format.buffer_size);
    buffer.buffer_size = format.buffer_size;

    //
    status_4 = unicap_start_capture(handle);
    if( !SUCCESS(status_4)){
        wxLogError(_T("Waiting for buffer, %x"), status_4);
        //exit(-1);
    }
    wxLogMessage(_T("  Started capture, %x"), status_4);

    status_5 = unicap_queue_buffer(handle, &buffer);
    if( !SUCCESS(status_5)){
        wxLogError(_T("Waiting for buffer, %x"), status_5);
        //exit(-1);
    }
    wxLogMessage(_T("  Queued buffer, %x"), status_5);

/*
    //while(frame_count < MAX_FRAMES){
    //    wxLogMessage(_T("  In Loop"))
        unicap_data_buffer_t * returned_buffer;
        status_6 = unicap_wait_buffer(handle, &returned_buffer);
        if( !SUCCESS(status_6))
        {
            wxLogError(_T("Waiting for buffer, %x"), status_6);
            //exit(-1);
        }
        wxLogMessage(_T("  Waited for buffer: %d, %x"), frame_count, status_6);
        frame_count++;
    //}
*/

    // Setup newframe callback
    //status_4 = unicap_register_callback( handle, UNICAP_EVENT_NEW_FRAME, (unicap_callback_t)new_frame, (void*)&frame_count );
    //if( !SUCCESS(status_4))
    //{
     //  wxLogError(_T("Couldn't register callback, %x"), status_4);
     //  exit(-1);
    //}

    // Start capturing video
    //status_5 = unicap_start_capture(handle);
    //if (!SUCCESS(status_5)){
    //    wxLogError(_T("  Couldn't start capture: %x"), status_5);
    //    exit(-1);
    //}

    unicap_close(handle);
    free(buffer.data);

    //wxLogMessage(_T("  OK: %x - %x - %x - %x"), status_1, status_2, status_3, status_4);


}

*/

//static void new_frame(unicap_event_t event, unicap_handle_t handle, unicap_data_buffer_t *buffer, void *usr_data){
//    volatile int *frame_count = ( volatile int * )usr_data;
//    *frame_count = *frame_count +1;
//}

//void get_format()

//int create_buffer(
