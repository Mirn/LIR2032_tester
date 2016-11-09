#ifndef __SIMPLE_FILTS__
#define __SIMPLE_FILTS__

#include <stdio.h>
#include <stdint.h>

static inline void printf_src_ver_simple_filts_h()
{
	printf(__DATE__"\t"__TIME__"\t"__FILE__"\r");
}

typedef struct {
	uint32_t  sum;
	uint32_t  cnt;
	uint32_t *buf;
	const uint32_t length;
} tfilt_avrg;

static inline uint32_t filt_avrg_calc(tfilt_avrg *filt, const uint32_t value)
{
	uint32_t pos = filt->cnt % filt->length;
	filt->cnt++;

	filt->sum -= filt->buf[pos];
	filt->buf[pos] = value;
	filt->sum += filt->buf[pos];

	return filt->sum / filt->length;
}

static inline void filt_avrg_init(tfilt_avrg *filt)
{
	filt->sum = 0;
	filt->cnt = 0;
	memset(filt->buf, 0, filt->length * sizeof(filt->buf[0]));
}

typedef struct {
	uint32_t accum;
	const uint16_t tau;
} tfilt_rc;

static inline void filt_rc_init(tfilt_rc *filt, const uint32_t value)
{
	filt->accum = value * filt->tau;
}

static inline uint32_t filt_rc_calc(tfilt_rc *filt, const uint32_t value)
{
	filt->accum = (((uint64_t)filt->accum) * (filt->tau - 1)) / filt->tau;
	filt->accum += value;
	return filt->accum / filt->tau;
}

#endif
