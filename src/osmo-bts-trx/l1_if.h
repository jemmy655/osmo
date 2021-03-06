#ifndef L1_IF_H_TRX
#define L1_IF_H_TRX

/* These types define the different channels on a multiframe.
 * Each channel has queues and can be activated individually.
 */
enum trx_chan_type {
	TRXC_IDLE = 0,
	TRXC_FCCH,
	TRXC_SCH,
	TRXC_BCCH,
	TRXC_RACH,
	TRXC_CCCH,
	TRXC_TCHF,
	TRXC_TCHH_0,
	TRXC_TCHH_1,
	TRXC_SDCCH4_0,
	TRXC_SDCCH4_1,
	TRXC_SDCCH4_2,
	TRXC_SDCCH4_3,
	TRXC_SDCCH8_0,
	TRXC_SDCCH8_1,
	TRXC_SDCCH8_2,
	TRXC_SDCCH8_3,
	TRXC_SDCCH8_4,
	TRXC_SDCCH8_5,
	TRXC_SDCCH8_6,
	TRXC_SDCCH8_7,
	TRXC_SACCHTF,
	TRXC_SACCHTH_0,
	TRXC_SACCHTH_1,
	TRXC_SACCH4_0,
	TRXC_SACCH4_1,
	TRXC_SACCH4_2,
	TRXC_SACCH4_3,
	TRXC_SACCH8_0,
	TRXC_SACCH8_1,
	TRXC_SACCH8_2,
	TRXC_SACCH8_3,
	TRXC_SACCH8_4,
	TRXC_SACCH8_5,
	TRXC_SACCH8_6,
	TRXC_SACCH8_7,
	TRXC_PDTCH,
	TRXC_PTCCH,
	_TRX_CHAN_MAX
};

/* States each channel on a multiframe */
struct trx_chan_state {
	/* scheduler */
	uint8_t			active;		/* Channel is active */
	ubit_t			*dl_bursts;	/* burst buffer for TX */
	sbit_t			*ul_bursts;	/* burst buffer for RX */
	uint32_t		ul_first_fn;	/* fn of first burst */
	uint8_t			ul_mask;	/* mask of received bursts */

	/* RSSI / TOA */
	uint8_t			rssi_num;	/* number of RSSI values */
	float			rssi_sum;	/* sum of RSSI values */
	uint8_t			toa_num;	/* number of TOA values */
	float			toa_sum;	/* sum of TOA values */

	/* loss detection */
	uint8_t			lost;		/* (SACCH) loss detection */

	/* mode */
	uint8_t			rsl_cmode, tch_mode; /* mode for TCH channels */

	/* AMR */
	uint8_t			codec[4];	/* 4 possible codecs for amr */
	int			codecs;		/* number of possible codecs */
	float			ber_sum;	/* sum of bit error rates */
	int			ber_num;	/* number of bit error rates */
	uint8_t			ul_ft;		/* current uplink FT index */
	uint8_t			dl_ft;		/* current downlink FT index */
	uint8_t			ul_cmr;		/* current uplink CMR index */
	uint8_t			dl_cmr;		/* current downlink CMR index */
	uint8_t			amr_loop;	/* if AMR loop is enabled */

	/* TCH/H */
	uint8_t			dl_ongoing_facch; /* FACCH/H on downlink */
	uint8_t			ul_ongoing_facch; /* FACCH/H on uplink */

	/* encryption */
	int			ul_encr_algo;	/* A5/x encry algo downlink */
	int			dl_encr_algo;	/* A5/x encry algo uplink */
	int			ul_encr_key_len;
	int			dl_encr_key_len;
	uint8_t			ul_encr_key[MAX_A5_KEY_LEN];
	uint8_t			dl_encr_key[MAX_A5_KEY_LEN];

	/* measurements */
	struct {
		uint8_t		clock;		/* cyclic clock counter */
		int8_t		rssi[32];	/* last RSSI values */
		int		rssi_count;	/* received RSSI values */
		int		rssi_valid_count; /* number of stored value */
		int		rssi_got_burst; /* any burst received so far */
		float		toa_sum;	/* sum of TOA values */
		int		toa_num;	/* number of TOA value */
	} meas;

	/* handover */
	uint8_t			ho_rach_detect;	/* if rach detection is on */
};

struct trx_config {
	uint8_t			poweron;	/* poweron(1) or poweroff(0) */
	int			poweron_sent;

	int			arfcn_valid;
	uint16_t		arfcn;
	int			arfcn_sent;

	int			tsc_valid;
	uint8_t			tsc;
	int			tsc_sent;

	int			bsic_valid;
	uint8_t			bsic;
	int			bsic_sent;

	int			rxgain_valid;
	int			rxgain;
	int			rxgain_sent;

	int			power_valid;
	int			power;
	int			power_oml;
	int			power_sent;

	int			maxdly_valid;
	int			maxdly;
	int			maxdly_sent;

	uint8_t			slotmask;

	int			slottype_valid[TRX_NR_TS];
	uint8_t			slottype[TRX_NR_TS];
	int			slottype_sent[TRX_NR_TS];
};

struct trx_l1h {
	struct llist_head	trx_ctrl_list;

	struct gsm_bts_trx	*trx;

	struct osmo_fd		trx_ofd_ctrl;
	struct osmo_timer_list	trx_ctrl_timer;
	struct osmo_fd		trx_ofd_data;

	/* transceiver config */
	struct trx_config	config;

	uint8_t			mf_index[TRX_NR_TS];	/* selected multiframe index */
	uint32_t		mf_last_fn[TRX_NR_TS];	/* last received frame */
	uint8_t			mf_period[TRX_NR_TS];	/* period of multiframe */
	const struct trx_sched_frame *mf_frames[TRX_NR_TS];	/* pointer to frame layout */

	/* Channel states for all channels on all timeslots */
	struct trx_chan_state	chan_states[TRX_NR_TS][_TRX_CHAN_MAX];
	struct llist_head	dl_prims[TRX_NR_TS];	/* Queue primitves for TX */
	uint8_t			ho_rach_detect[TRX_NR_TS][TS_MAX_LCHAN];
};

struct trx_l1h *l1if_open(struct gsm_bts_trx *trx);
void l1if_close(struct trx_l1h *l1h);
void l1if_reset(struct trx_l1h *l1h);
int check_transceiver_availability(struct gsm_bts *bts, int avail);
int l1if_provision_transceiver_trx(struct trx_l1h *l1h);
int l1if_provision_transceiver(struct gsm_bts *bts);
int l1if_mph_time_ind(struct gsm_bts *bts, uint32_t fn);
void l1if_fill_meas_res(struct osmo_phsap_prim *l1sap, uint8_t chan_nr, float ta,
	float ber, float rssi);
int l1if_process_meas_res(struct gsm_bts_trx *trx, uint8_t tn, uint32_t fn, uint8_t chan_nr,
	int n_errors, int n_bits_total, float rssi, float toa);

#endif /* L1_IF_H_TRX */
