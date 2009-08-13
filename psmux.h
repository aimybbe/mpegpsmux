/* 
 * Copyright 2006 BBC and Fluendo S.A. 
 */

#ifndef __PSMUX_H__
#define __PSMUX_H__

#include <glib.h>

#include "psmuxcommon.h"
#include "psmuxstream.h"

G_BEGIN_DECLS

#define PSMUX_MAX_ES_INFO_LENGTH ((1 << 12) - 1)

typedef gboolean (*PsMuxWriteFunc) (guint8 *data, guint len, void *user_data);

struct PsMux {
  GList *streams;    /* PsMuxStream* array of all streams */
  guint nb_streams;
  guint nb_private_streams;
  PsMuxStreamIdInfo id_info; /* carrying the info which ids are used */

  /* timestamps: pts */ 
  GstClockTime pts;

  guint32 pes_cnt; /* # of pes that has been created */
  guint16 pes_max_payload; /* maximum payload size in pes packets */

  guint64 bit_size;  /* accumulated bit size of processed data */
  guint bit_rate;  /* bit rate */ 
  GstClockTime bit_pts; /* last time the bit_rate is updated */

  guint pack_hdr_freq; /* PS pack header frequency */
  GstClockTime pack_hdr_pts; /* last time a pack header is written */

  guint sys_hdr_freq; /* system header frequency */ 
  GstClockTime sys_hdr_pts; /* last time a system header is written */

  guint psm_freq; /* program stream map frequency */ 
  GstClockTime psm_pts; /* last time a psm is written */

  guint8 packet_buf[PSMUX_MAX_PACKET_LEN];
  guint packet_bytes_written; /* # of bytes written in the buf */
  PsMuxWriteFunc write_func;
  void *write_func_data;

  /* Scratch space for writing ES_info descriptors */
  guint8 es_info_buf[PSMUX_MAX_ES_INFO_LENGTH];

  /* bounds in system header */ 
  guint8 audio_bound;
  guint8 video_bound;
  guint32 rate_bound;
};

/* create/free new muxer session */
PsMux *		psmux_new 			(void);
void 		psmux_free 			(PsMux *mux);

/* Setting muxing session properties */
void 		psmux_set_write_func 		(PsMux *mux, PsMuxWriteFunc func, void *user_data);

/* stream management */
PsMuxStream *	psmux_create_stream 		(PsMux *mux, PsMuxStreamType stream_type);

/* writing stuff */
gboolean 	psmux_write_stream_packet 	(PsMux *mux, PsMuxStream *stream); 
gboolean	psmux_write_end_code		(PsMux *mux);

G_END_DECLS

#endif
