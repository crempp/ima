/***************************************************************
 * Name:      video.h
 * Purpose:   Code for grabbing the video
 * Author:    Chad Rempp (crempp@gmail.com)
 * Created:   2008-10-28
 * Copyright: Chad Rempp (http://www.tetractys.org)
 * License:
 **************************************************************/
#ifndef VIDEO_H_INCLUDED
#define VIDEO_H_INCLUDED

#include <unicap.h>
#include <unicap_status.h>
#include <algorithm>
#include <list>
#include <string>
#include <vector>

#define MAX_DEVICES 4
#define NUM_BUFFERS 10
#define MAX_FRAMES  15

/*struct cam_device{
    int exists;
    wxString dev_name;
    unicap_handle_t handle;
    unicap_device_t device;
    int dev_number;

    unicap_data_buffer_t buffer;
    unicap_data_buffer_t* returned_buffer;
};*/

void list_devices(void);
void load_device(int dev_num);
//static void new_frame(unicap_event_t event, unicap_handle_t handle, unicap_data_buffer_t *buffer, void *usr_data);

class camDataBuffer{
	public:
		camDataBuffer();
		camDataBuffer( void *ptr );

		camDataBuffer& operator= ( void *DataPtr );

		void *get_ptr() { return (void *)m_unicap_buffer->data; };
		bool operator== ( unicap_data_buffer_t *rhs ) { return m_unicap_buffer == rhs; }

		friend class camDevice;
	private:
		unicap_data_buffer_t *GetUnicapBuffer() { return m_unicap_buffer; }


	private:
		unicap_data_buffer_t *m_unicap_buffer;

};

class camDevice
{

	public:
		camDevice() { strcpy( m_device.identifier, "" ); }
		camDevice( wxString id );

		static list<camDevice* > EnumerateDevices();

		unicap_status_t Open();
		unicap_status_t Close();

		unicap_status_t StartCapture();
		unicap_status_t StopCapture();

		unicap_status_t QueueBuffer( CUnicapDataBuffer &b );
		unicap_status_t WaitBuffer( CUnicapDataBuffer **b );

		char *c_str() {return( m_device.identifier );}

	private:
		unicap_device_t m_device;

		unicap_handle_t m_handle;

		vector<CUnicapDataBuffer>m_queued_buffers;
};

#endif // VIDEO_H_INCLUDED
