#ifndef __CUSTOM_NVME_H
#define __CUSTOM_NVME_H

#include <linux/nvme_ioctl.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define log_err(format, ...)                                                       \
do {                                                                         \
    fprintf(stdout, "[LOG] %s:%d: " format "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
} while (0)

#ifndef CONFIG_NVME_URING_CMD
struct nvme_uring_cmd {
	__u8	opcode;
	__u8	flags;
	__u16	rsvd1;
	__u32	nsid;
	__u32	cdw2;
	__u32	cdw3;
	__u64	metadata;
	__u64	addr;
	__u32	metadata_len;
	__u32	data_len;
	__u32	cdw10;
	__u32	cdw11;
	__u32	cdw12;
	__u32	cdw13;
	__u32	cdw14;
	__u32	cdw15;
	__u32	timeout_ms;
	__u32   rsvd2;
};

#define NVME_URING_CMD_IO	_IOWR('N', 0x80, struct nvme_uring_cmd)
#define NVME_URING_CMD_IO_VEC	_IOWR('N', 0x81, struct nvme_uring_cmd)
#endif /* CONFIG_NVME_URING_CMD */

#define NVME_DEFAULT_IOCTL_TIMEOUT 0
#define NVME_IDENTIFY_DATA_SIZE 4096
#define NVME_IDENTIFY_CSI_SHIFT 24
#define NVME_NQN_LENGTH	256

#define NVME_PI_APP_DISABLE 0xFFFF
#define NVME_PI_REF_DISABLE 0xFFFFFFFF

#define NVME_ZNS_ZRA_REPORT_ZONES 0
#define NVME_ZNS_ZRAS_FEAT_ERZ (1 << 16)
#define NVME_ZNS_ZSA_RESET 0x4
#define NVME_ZONE_TYPE_SEQWRITE_REQ 0x2

#define NVME_ATTRIBUTE_DEALLOCATE (1 << 2)

enum nvme_identify_cns {
	NVME_IDENTIFY_CNS_NS		= 0x00,
	NVME_IDENTIFY_CNS_CTRL		= 0x01,
	NVME_IDENTIFY_CNS_CSI_NS	= 0x05,
	NVME_IDENTIFY_CNS_CSI_CTRL	= 0x06,
};

enum nvme_csi {
	NVME_CSI_NVM			= 0,
	NVME_CSI_KV			= 1,
	NVME_CSI_ZNS			= 2,
};

enum nvme_admin_opcode {
	nvme_admin_identify		= 0x06,
};

enum nvme_io_opcode {
	nvme_cmd_flush			= 0x00,
	nvme_cmd_write			= 0x01,
	nvme_cmd_read			= 0x02,
	nvme_cmd_write_uncor		= 0x04,
	nvme_cmd_compare		= 0x05,
	nvme_cmd_write_zeroes		= 0x08,
	nvme_cmd_dsm			= 0x09,
	nvme_cmd_verify			= 0x0c,
	nvme_cmd_io_mgmt_recv		= 0x12,
	nvme_zns_cmd_mgmt_send		= 0x79,
	nvme_zns_cmd_mgmt_recv		= 0x7a,
};

enum nvme_zns_zs {
	NVME_ZNS_ZS_EMPTY		= 0x1,
	NVME_ZNS_ZS_IMPL_OPEN		= 0x2,
	NVME_ZNS_ZS_EXPL_OPEN		= 0x3,
	NVME_ZNS_ZS_CLOSED		= 0x4,
	NVME_ZNS_ZS_READ_ONLY		= 0xd,
	NVME_ZNS_ZS_FULL		= 0xe,
	NVME_ZNS_ZS_OFFLINE		= 0xf,
};

enum nvme_id_ctrl_ctratt {
	NVME_CTRL_CTRATT_ELBAS		= 1 << 15,
};

enum {
	NVME_ID_NS_NVM_STS_MASK		= 0x7f,
	NVME_ID_NS_NVM_GUARD_SHIFT	= 7,
	NVME_ID_NS_NVM_GUARD_MASK	= 0x3,
};

enum {
	NVME_NVM_NS_16B_GUARD		= 0,
	NVME_NVM_NS_32B_GUARD		= 1,
	NVME_NVM_NS_64B_GUARD		= 2,
};

struct nvme_data {
	__u32 nsid;
	__u32 lba_shift;
	__u32 lba_size;
	__u32 lba_ext;
	__u16 ms;
	__u16 pi_size;
	__u8 pi_type;
	__u8 guard_type;
	__u8 pi_loc;
};

enum nvme_id_ns_dps {
	NVME_NS_DPS_PI_NONE		= 0,
	NVME_NS_DPS_PI_TYPE1		= 1,
	NVME_NS_DPS_PI_TYPE2		= 2,
	NVME_NS_DPS_PI_TYPE3		= 3,
	NVME_NS_DPS_PI_MASK		= 7 << 0,
	NVME_NS_DPS_PI_FIRST		= 1 << 3,
};

enum nvme_io_control_flags {
	NVME_IO_PRINFO_PRCHK_REF	= 1U << 26,
	NVME_IO_PRINFO_PRCHK_APP	= 1U << 27,
	NVME_IO_PRINFO_PRCHK_GUARD	= 1U << 28,
	NVME_IO_PRINFO_PRACT		= 1U << 29,
};

struct nvme_pi_data {
	__u32 interval;
	__u32 io_flags;
	__u16 apptag;
	__u16 apptag_mask;
};

struct nvme_lbaf {
	__le16			ms;
	__u8			ds;
	__u8			rp;
};

struct nvme_id_ns {
	__le64			nsze;
	__le64			ncap;
	__le64			nuse;
	__u8			nsfeat;
	__u8			nlbaf;
	__u8			flbas;
	__u8			mc;
	__u8			dpc;
	__u8			dps;
	__u8			nmic;
	__u8			rescap;
	__u8			fpi;
	__u8			dlfeat;
	__le16			nawun;
	__le16			nawupf;
	__le16			nacwu;
	__le16			nabsn;
	__le16			nabo;
	__le16			nabspf;
	__le16			noiob;
	__u8			nvmcap[16];
	__le16			npwg;
	__le16			npwa;
	__le16			npdg;
	__le16			npda;
	__le16			nows;
	__le16			mssrl;
	__le32			mcl;
	__u8			msrc;
	__u8			rsvd81[11];
	__le32			anagrpid;
	__u8			rsvd96[3];
	__u8			nsattr;
	__le16			nvmsetid;
	__le16			endgid;
	__u8			nguid[16];
	__u8			eui64[8];
	struct nvme_lbaf	lbaf[64];
	__u8			vs[3712];
};

struct nvme_id_psd {
	__le16			mp;
	__u8			rsvd2;
	__u8			flags;
	__le32			enlat;
	__le32			exlat;
	__u8			rrt;
	__u8			rrl;
	__u8			rwt;
	__u8			rwl;
	__le16			idlp;
	__u8			ips;
	__u8			rsvd19;
	__le16			actp;
	__u8			apws;
	__u8			rsvd23[9];
};

struct nvme_id_ctrl {
	__le16			vid;
	__le16			ssvid;
	char			sn[20];
	char			mn[40];
	char			fr[8];
	__u8			rab;
	__u8			ieee[3];
	__u8			cmic;
	__u8			mdts;
	__le16			cntlid;
	__le32			ver;
	__le32			rtd3r;
	__le32			rtd3e;
	__le32			oaes;
	__le32			ctratt;
	__le16			rrls;
	__u8			rsvd102[9];
	__u8			cntrltype;
	__u8			fguid[16];
	__le16			crdt1;
	__le16			crdt2;
	__le16			crdt3;
	__u8			rsvd134[119];
	__u8			nvmsr;
	__u8			vwci;
	__u8			mec;
	__le16			oacs;
	__u8			acl;
	__u8			aerl;
	__u8			frmw;
	__u8			lpa;
	__u8			elpe;
	__u8			npss;
	__u8			avscc;
	__u8			apsta;
	__le16			wctemp;
	__le16			cctemp;
	__le16			mtfa;
	__le32			hmpre;
	__le32			hmmin;
	__u8			tnvmcap[16];
	__u8			unvmcap[16];
	__le32			rpmbs;
	__le16			edstt;
	__u8			dsto;
	__u8			fwug;
	__le16			kas;
	__le16			hctma;
	__le16			mntmt;
	__le16			mxtmt;
	__le32			sanicap;
	__le32			hmminds;
	__le16			hmmaxd;
	__le16			nsetidmax;
	__le16			endgidmax;
	__u8			anatt;
	__u8			anacap;
	__le32			anagrpmax;
	__le32			nanagrpid;
	__le32			pels;
	__le16			domainid;
	__u8			rsvd358[10];
	__u8			megcap[16];
	__u8			rsvd384[128];
	__u8			sqes;
	__u8			cqes;
	__le16			maxcmd;
	__le32			nn;
	__le16			oncs;
	__le16			fuses;
	__u8			fna;
	__u8			vwc;
	__le16			awun;
	__le16			awupf;
	__u8			icsvscc;
	__u8			nwpc;
	__le16			acwu;
	__le16			ocfs;
	__le32			sgls;
	__le32			mnan;
	__u8			maxdna[16];
	__le32			maxcna;
	__u8			rsvd564[204];
	char			subnqn[NVME_NQN_LENGTH];
	__u8			rsvd1024[768];

	/* Fabrics Only */
	__le32			ioccsz;
	__le32			iorcsz;
	__le16			icdoff;
	__u8			fcatt;
	__u8			msdbd;
	__le16			ofcs;
	__u8			dctype;
	__u8			rsvd1807[241];

	struct nvme_id_psd	psd[32];
	__u8			vs[1024];
};

struct nvme_nvm_id_ns {
	__le64			lbstm;
	__u8			pic;
	__u8			rsvd9[3];
	__le32			elbaf[64];
	__u8			rsvd268[3828];
};

struct nvme_dsm_range {
	__le32	cattr;
	__le32	nlb;
	__le64	slba;
};

struct nvme_dsm {
	__u32 nr_ranges;
	struct nvme_dsm_range range[];
};

struct nvme_cmd_ext_io_opts {
	__u32 io_flags;
	__u16 apptag;
	__u16 apptag_mask;
};


struct nvme_fdp_ruh_status_desc {
	__u16 pid;
	__u16 ruhid;
	__u32 earutr;
	__u64 ruamw;
	__u8  rsvd16[16];
};

struct nvme_fdp_ruh_status {
	__u8  rsvd0[14];
	__le16 nruhsd;
	struct nvme_fdp_ruh_status_desc ruhss[];
};

#define NVME_IOCTL_ID		_IO('N', 0x40)
#define NVME_IOCTL_ADMIN_CMD	_IOWR('N', 0x41, struct nvme_admin_cmd)
#define NVME_IOCTL_SUBMIT_IO	_IOW('N', 0x42, struct nvme_user_io)
#define NVME_IOCTL_IO_CMD	_IOWR('N', 0x43, struct nvme_passthru_cmd)
#define NVME_IOCTL_RESET	_IO('N', 0x44)
#define NVME_IOCTL_SUBSYS_RESET	_IO('N', 0x45)
#define NVME_IOCTL_RESCAN	_IO('N', 0x46)
#define NVME_IOCTL_ADMIN64_CMD	_IOWR('N', 0x47, struct nvme_passthru_cmd64)
#define NVME_IOCTL_IO64_CMD	_IOWR('N', 0x48, struct nvme_passthru_cmd64)

#define STREAMS_DIR_DTYPE	1
#define FDP_DIR_DTYPE		2
#define FIO_MAX_DP_IDS 		128
#define DP_MAX_SCHEME_ENTRIES	32

/*
 * How fio chooses what placement identifier to use next. Choice of
 * uniformly random, or roundrobin.
 */
enum {
	FIO_DP_RANDOM	= 0x1,
	FIO_DP_RR	= 0x2,
	FIO_DP_SCHEME	= 0x3,
};

enum {
	FIO_DP_NONE	= 0x0,
	FIO_DP_FDP	= 0x1,
	FIO_DP_STREAMS	= 0x2,
};

struct fio_ruhs_info {
	uint32_t nr_ruhs;
	uint32_t pli_loc;
	uint16_t plis[];
};

struct fio_ruhs_scheme_entry {
	unsigned long long start_offset;
	unsigned long long end_offset;
	uint16_t pli;
};

struct fio_ruhs_scheme {
	uint16_t nr_schemes;
	struct fio_ruhs_scheme_entry scheme_entries[DP_MAX_SCHEME_ENTRIES];
};

struct io_u {
	struct fio_file *file;
	unsigned int flags;

	unsigned long long buflen;
	unsigned long long offset;	/* is really ->xfer_offset... */
	void *buf;

	unsigned int error;
	void *engine_data;

	/*
	 * Callback for io completion
	 */
	int (*end_io)(struct thread_data *, struct io_u **);

	uint32_t dtype;
	uint32_t dspec;

    void *mmap_data;
};


static inline int ilog2(uint32_t i)
{
	int log = -1;

	while (i) {
		i >>= 1;
		log++;
	}
	return log;
}

static int nvme_identify(int fd, __u32 nsid, enum nvme_identify_cns cns,
			 enum nvme_csi csi, void *data)
{
	struct nvme_passthru_cmd cmd = {
		.opcode         = nvme_admin_identify,
		.nsid           = nsid,
		.addr           = (__u64)(uintptr_t)data,
		.data_len       = NVME_IDENTIFY_DATA_SIZE,
		.cdw10          = cns,
		.cdw11          = csi << NVME_IDENTIFY_CSI_SHIFT,
		.timeout_ms     = NVME_DEFAULT_IOCTL_TIMEOUT,
	};

	return ioctl(fd, NVME_IOCTL_ADMIN_CMD, &cmd);
}

int nvme_get_info(const char *file_name, __u64 *nlba,
		      struct nvme_data *data)
{
	struct nvme_id_ns ns;
	struct nvme_id_ctrl ctrl;
	struct nvme_nvm_id_ns nvm_ns;
	int namespace_id;
	int fd, err;
	__u32 format_idx, elbaf;

	fd = open(file_name, O_RDONLY);
	if (fd < 0)
		return -errno;

	namespace_id = ioctl(fd, NVME_IOCTL_ID);
	if (namespace_id < 0) {
		err = -errno;
		log_err("%s: failed to fetch namespace-id\n", file_name);
		goto out;
	}

	err = nvme_identify(fd, 0, NVME_IDENTIFY_CNS_CTRL, NVME_CSI_NVM, &ctrl);
	if (err) {
		log_err("%s: failed to fetch identify ctrl\n", file_name);
		goto out;
	}

	/*
	 * Identify namespace to get namespace-id, namespace size in LBA's
	 * and LBA data size.
	 */
	err = nvme_identify(fd, namespace_id, NVME_IDENTIFY_CNS_NS,
				NVME_CSI_NVM, &ns);
	if (err) {
		log_err("%s: failed to fetch identify namespace\n",
			file_name);
		goto out;
	}

	data->nsid = namespace_id;

	/*
	 * 16 or 64 as maximum number of supported LBA formats.
	 * From flbas bit 0-3 indicates lsb and bit 5-6 indicates msb
	 * of the format index used to format the namespace.
	 */
	if (ns.nlbaf < 16)
		format_idx = ns.flbas & 0xf;
	else
		format_idx = (ns.flbas & 0xf) + (((ns.flbas >> 5) & 0x3) << 4);

	data->lba_size = 1 << ns.lbaf[format_idx].ds;
	data->ms = ns.lbaf[format_idx].ms;

	/* Check for end to end data protection support */
	if (data->ms && (ns.dps & NVME_NS_DPS_PI_MASK))
		data->pi_type = (ns.dps & NVME_NS_DPS_PI_MASK);

	if (!data->pi_type)
		goto check_elba;

	if (ctrl.ctratt & NVME_CTRL_CTRATT_ELBAS) {
		err = nvme_identify(fd, namespace_id, NVME_IDENTIFY_CNS_CSI_NS,
					NVME_CSI_NVM, &nvm_ns);
		if (err) {
			log_err("%s: failed to fetch identify nvm namespace\n",
				file_name);
			goto out;
		}

		elbaf = nvm_ns.elbaf[format_idx];

		/* Currently we don't support storage tags */
		if (elbaf & NVME_ID_NS_NVM_STS_MASK) {
			log_err("%s: Storage tag not supported\n",
				file_name);
			err = -ENOTSUP;
			goto out;
		}
	} else {
		data->guard_type = NVME_NVM_NS_16B_GUARD;
	}

	data->pi_loc = (ns.dps & NVME_NS_DPS_PI_FIRST);

check_elba:
	/*
	 * Bit 4 for flbas indicates if metadata is transferred at the end of
	 * logical block creating an extended LBA.
	 */
	if (data->ms && ((ns.flbas >> 4) & 0x1))
		data->lba_ext = data->lba_size + data->ms;
	else
		data->lba_shift = ilog2(data->lba_size);

	*nlba = ns.nsze;

out:
	close(fd);
	return err;
}

static inline int nvme_fdp_reclaim_unit_handle_status(int fd, __u32 nsid,
						      __u32 data_len, void *data)
{
	struct nvme_passthru_cmd cmd = {
		.opcode		= nvme_cmd_io_mgmt_recv,
		.nsid		= nsid,
		.addr		= (__u64)(uintptr_t)data,
		.data_len 	= data_len,
		.cdw10		= 1,
		.cdw11		= (data_len >> 2) - 1,
	};

	return ioctl(fd, NVME_IOCTL_IO_CMD, &cmd);
}

int nvme_iomgmt_ruhs(const char *file_name,
			 struct nvme_fdp_ruh_status *ruhs, __u32 bytes, struct nvme_data *data)
{
	int fd, ret;

	fd = open(file_name, O_RDONLY | O_LARGEFILE);
	if (fd < 0)
		return -errno;

	ret = nvme_fdp_reclaim_unit_handle_status(fd, data->nsid, bytes, ruhs);
	if (ret) {
		log_err("%s: nvme_fdp_reclaim_unit_handle_status failed, err=%d\n",
			file_name, ret);
		errno = ENOTSUP;
	} else
		errno = 0;

	ret = -errno;
	close(fd);

	return ret;
}

static int fdp_fetch_ruhs(const char *file_name,
				     struct fio_ruhs_info *fruhs_info, struct nvme_data *data)
{
	struct nvme_fdp_ruh_status *ruhs;
	int bytes, nr_ruhs, ret, i;

	nr_ruhs = fruhs_info->nr_ruhs;
	bytes = sizeof(*ruhs) + fruhs_info->nr_ruhs * sizeof(struct nvme_fdp_ruh_status_desc);

	ruhs = (struct nvme_fdp_ruh_status *)calloc(1, bytes);
	if (!ruhs)
		return -ENOMEM;

	ret = nvme_iomgmt_ruhs(file_name, ruhs, bytes, data);
	if (ret)
		goto free;

	fruhs_info->nr_ruhs = ruhs->nruhsd;
	for (i = 0; i < nr_ruhs; i++)
		fruhs_info->plis[i] = ruhs->ruhss[i].pid;

free:
	free(ruhs);
	return ret;
}


static int fdp_ruh_info(const char *file_name,
			struct fio_ruhs_info *ruhs)
{
    struct nvme_data *data;
	int ret = -EINVAL;

    data = (struct nvme_data *)calloc(1, sizeof(*data));

    ret = fdp_fetch_ruhs(file_name, ruhs, data);
    if (ret < 0) {
        log_err("%s: fdp fetch ruhs failed (%d)\n",
            file_name, errno);
    }

	return ret;
}

int dp_nr_ids = 1;
int dp_ids[1] = {0};
int dp_id_select = FIO_DP_RR;
const char *dp_scheme_file = "";

static int init_ruh_info(const char *file_name, struct fio_ruhs_info *ruhs_info)
{
	struct fio_ruhs_info *ruhs, *tmp;
	uint32_t nr_ruhs;
	int i, ret;

	/* set up the data structure used for FDP to work with the supplied stream IDs */
		ruhs = (struct fio_ruhs_info *)calloc(1, sizeof(*ruhs) + dp_nr_ids * sizeof(*ruhs->plis));
		if (!ruhs)
			return -ENOMEM;

		ruhs->nr_ruhs = dp_nr_ids;
		for (int i = 0; i < ruhs->nr_ruhs; i++)
			ruhs->plis[i] = dp_ids[i];

		ruhs_info = ruhs;
		return 0;

	/*
	 * Since we don't know the actual number of ruhs. Only fetch the header.
	 * We will reallocate this buffer and then fetch all the ruhs again.
	 */
	ruhs = (struct fio_ruhs_info *)calloc(1, sizeof(*ruhs));
	ret = fdp_ruh_info(file_name, ruhs);

	if (ret) {
		log_err("fio: ruh info failed for %s (%d)\n",
			file_name, -ret);
		goto out;
	}

	nr_ruhs = ruhs->nr_ruhs;
	ruhs = (struct fio_ruhs_info *)realloc(ruhs, sizeof(*ruhs) + nr_ruhs * sizeof(*ruhs->plis));
    
	if (!ruhs) {
		log_err("fio: ruhs buffer realloc failed for %s\n",
			file_name);
		ret = -ENOMEM;
		goto out;
	}

	ruhs->nr_ruhs = nr_ruhs;
	ret = fdp_ruh_info(file_name, ruhs);
	if (ret) {
		log_err("fio: ruh info failed for %s (%d)\n",
			file_name, -ret);
		goto out;
	}

	if (dp_nr_ids == 0) {
		if (ruhs->nr_ruhs > FIO_MAX_DP_IDS)
			ruhs->nr_ruhs = FIO_MAX_DP_IDS;
	} else {
		for (i = 0; i < dp_nr_ids; i++) {
			if (dp_ids[i] >= ruhs->nr_ruhs) {
				log_err("fio: for %s PID index %d must be smaller than %d\n",
					file_name, dp_ids[i],
					ruhs->nr_ruhs);
				ret = -EINVAL;
				goto out;
			}
		}
		ruhs->nr_ruhs = dp_nr_ids;
	}

	tmp = (struct fio_ruhs_info *)calloc(1, sizeof(*tmp) + ruhs->nr_ruhs * sizeof(*tmp->plis));
	if (!tmp) {
		ret = -ENOMEM;
		goto out;
	}

	if (dp_nr_ids == 0) {
		for (i = 0; i < ruhs->nr_ruhs; i++)
			tmp->plis[i] = ruhs->plis[i];

		tmp->nr_ruhs = ruhs->nr_ruhs;
		ruhs_info = tmp;
		free(ruhs);

		return 0;
	}

	tmp->nr_ruhs = dp_nr_ids;
	for (i = 0; i < dp_nr_ids; i++)
		tmp->plis[i] = ruhs->plis[dp_ids[i]];

	ruhs_info = tmp;

out:
	free(ruhs);
	return ret;
}

static int init_ruh_scheme(struct fio_ruhs_scheme *ruhs_scheme)
{
	struct fio_ruhs_scheme *ruh_scheme;
	FILE *scheme_fp;
	unsigned long long start, end;
	uint16_t pli;
	int ret = 0;

	if (dp_id_select != FIO_DP_SCHEME)
		return 0;

	/* Get the scheme from the file */
	scheme_fp = fopen(dp_scheme_file, "r");

	if (!scheme_fp) {
		log_err("fio: ruh scheme failed to open scheme file %s\n",
			dp_scheme_file);
		ret = -errno;
		goto out;
	}

	ruh_scheme = (struct fio_ruhs_scheme *)calloc(1, sizeof(*ruh_scheme));
	if (!ruh_scheme) {
		ret = -ENOMEM;
		goto out_with_close_fp;
	}

	for (int i = 0;
		i < DP_MAX_SCHEME_ENTRIES && fscanf(scheme_fp, "%llu,%llu,%hu\n", &start, &end, &pli) == 3;
		i++) {

		ruh_scheme->scheme_entries[i].start_offset = start;
		ruh_scheme->scheme_entries[i].end_offset = end;
		ruh_scheme->scheme_entries[i].pli = pli;
		ruh_scheme->nr_schemes++;
	}

	if (fscanf(scheme_fp, "%llu,%llu,%hu\n", &start, &end, &pli) == 3)
		log_err("fio: too many scheme entries in %s. Only the first %d scheme entries are applied\n",
			 dp_scheme_file,
			 DP_MAX_SCHEME_ENTRIES);

	ruhs_scheme = ruh_scheme;

out_with_close_fp:
	fclose(scheme_fp);

out:
	return ret;
}

// int dp_init()
// {
//     int ret;
//     ret = init_ruh_info();
//     if (ret)
//         goto out;
//
//     ret = init_ruh_scheme();
//
// out:
// 	return ret;
// }

void fdp_free_ruhs_info(struct fio_ruhs_info *ruhs_info, struct fio_ruhs_scheme *ruhs_scheme)
{
	if (!ruhs_info)
		return;
	free(ruhs_info);
	ruhs_info = NULL;

	if (!ruhs_scheme)
		return;
	free(ruhs_scheme);
	ruhs_scheme = NULL;
}

void dp_fill_dspec_data(struct fio_ruhs_info *ruhs, struct fio_ruhs_scheme *ruhs_scheme, struct io_u *io_u)
{
	struct fio_file *f = io_u->file;
	int dspec;

	if (!ruhs) {
		io_u->dtype = 0;
		io_u->dspec = 0;
		return;
	}

	if (dp_id_select == FIO_DP_RR) {
		if (ruhs->pli_loc >= ruhs->nr_ruhs)
			ruhs->pli_loc = 0;

		dspec = ruhs->plis[ruhs->pli_loc++];
	} else if (dp_id_select == FIO_DP_SCHEME) {
		struct fio_ruhs_scheme *ruhs_scheme = ruhs_scheme;
		unsigned long long offset = io_u->offset;
		int i;

		for (i = 0; i < ruhs_scheme->nr_schemes; i++) {
			if (offset >= ruhs_scheme->scheme_entries[i].start_offset &&
			    offset < ruhs_scheme->scheme_entries[i].end_offset) {
				dspec = ruhs_scheme->scheme_entries[i].pli;
				break;
			}
		}

		/*
		 * If the write offset is not affected by any scheme entry,
		 * 0(default RUH) will be assigned to dspec
		 */
		if (i == ruhs_scheme->nr_schemes)
			dspec = 0;
	} else {
		ruhs->pli_loc = rand() % (ruhs->nr_ruhs - 1);
		dspec = ruhs->plis[ruhs->pli_loc];
	}

	io_u->dtype = FDP_DIR_DTYPE;
	io_u->dspec = dspec;

	log_err("dtype set to 0x%x, dspec set to 0x%x\n", io_u->dtype, io_u->dspec);
}

static inline __u64 get_slba(struct nvme_data *data, __u64 offset)
{
	if (data->lba_ext)
		return offset / data->lba_ext;

	return offset >> data->lba_shift;
}

static inline __u32 get_nlb(struct nvme_data *data, __u64 len)
{
	if (data->lba_ext)
		return len / data->lba_ext - 1;

	return (len >> data->lba_shift) - 1;
}

int nvme_uring_cmd_prep(struct nvme_uring_cmd *cmd, struct io_u *io_u,
			    struct iovec *iov, struct nvme_dsm *dsm,
			    uint8_t read_opcode, uint8_t write_opcode,
			    unsigned int cdw12_flags, struct nvme_data *data)
{
	__u64 slba;
	__u32 nlb;

	memset(cmd, 0, sizeof(struct nvme_uring_cmd));
    cmd->opcode = write_opcode;

	slba = get_slba(data, io_u->offset);
	nlb = get_nlb(data, io_u->buflen);

	/* cdw10 and cdw11 represent starting lba */
	cmd->cdw10 = slba & 0xffffffff;
	cmd->cdw11 = slba >> 32;
	/* cdw12 represent number of lba's for read/write */
	cmd->cdw12 = nlb | (io_u->dtype << 20) | cdw12_flags;
	cmd->cdw13 = io_u->dspec << 16;

	if (iov) {
		iov->iov_base = io_u->buf;
		iov->iov_len = io_u->buflen;
		cmd->addr = (__u64)(uintptr_t)iov;
		cmd->data_len = 1;
	} else {
		/* no buffer for write zeroes */
		if (cmd->opcode != nvme_cmd_write_zeroes)
			cmd->addr = (__u64)(uintptr_t)io_u->buf;
		else
			cmd->addr = (__u64)(uintptr_t)NULL;
		cmd->data_len = io_u->buflen;
	}

	if (data->lba_shift && data->ms) {
		cmd->metadata = (__u64)(uintptr_t)io_u->mmap_data;
		cmd->metadata_len = (nlb + 1) * data->ms;
	}
	cmd->nsid = data->nsid;
	return 0;
}

static void nvme_pi_fill(struct nvme_uring_cmd *cmd, struct io_u *io_u,
		      struct nvme_cmd_ext_io_opts *opts, struct nvme_data *data)
{
	__u64 slba;
	slba = get_slba(data, io_u->offset);
	cmd->cdw12 |= opts->io_flags;
}

static int nvme_open_file(void)
{
    struct nvme_data *data = NULL;
    unsigned int lba_size = 0;
    __u64 nlba = 0;
    int ret;

    /* Store the namespace-id and lba size. */
    data = (struct nvme_data *)calloc(1, sizeof(struct nvme_data));
    ret = nvme_get_info("/dev/ng1n1", &nlba, data);
    if (ret) {
        free(data);
        return ret;
    }

    lba_size = data->lba_ext ? data->lba_ext : data->lba_size;
}

#ifdef __cplusplus
}
#endif

#endif
