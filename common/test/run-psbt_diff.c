#include "config.h"
#include <assert.h>
#include <bitcoin/chainparams.h>
#include <bitcoin/psbt.h>
#include <bitcoin/tx.h>
#include <common/setup.h>
#include <stdio.h>
#include <wire/wire.h>
#include "../amount.c"
#include "../psbt_open.c"

/* AUTOGENERATED MOCKS START */
/* Generated stub for fromwire */
const u8 *fromwire(const u8 **cursor UNNEEDED, size_t *max UNNEEDED, void *copy UNNEEDED, size_t n UNNEEDED)
{ fprintf(stderr, "fromwire called!\n"); abort(); }
/* Generated stub for fromwire_bool */
bool fromwire_bool(const u8 **cursor UNNEEDED, size_t *max UNNEEDED)
{ fprintf(stderr, "fromwire_bool called!\n"); abort(); }
/* Generated stub for fromwire_fail */
void *fromwire_fail(const u8 **cursor UNNEEDED, size_t *max UNNEEDED)
{ fprintf(stderr, "fromwire_fail called!\n"); abort(); }
/* Generated stub for fromwire_secp256k1_ecdsa_signature */
void fromwire_secp256k1_ecdsa_signature(const u8 **cursor UNNEEDED, size_t *max UNNEEDED,
					secp256k1_ecdsa_signature *signature UNNEEDED)
{ fprintf(stderr, "fromwire_secp256k1_ecdsa_signature called!\n"); abort(); }
/* Generated stub for fromwire_sha256 */
void fromwire_sha256(const u8 **cursor UNNEEDED, size_t *max UNNEEDED, struct sha256 *sha256 UNNEEDED)
{ fprintf(stderr, "fromwire_sha256 called!\n"); abort(); }
/* Generated stub for fromwire_tal_arrn */
u8 *fromwire_tal_arrn(const tal_t *ctx UNNEEDED,
		       const u8 **cursor UNNEEDED, size_t *max UNNEEDED, size_t num UNNEEDED)
{ fprintf(stderr, "fromwire_tal_arrn called!\n"); abort(); }
/* Generated stub for fromwire_u16 */
u16 fromwire_u16(const u8 **cursor UNNEEDED, size_t *max UNNEEDED)
{ fprintf(stderr, "fromwire_u16 called!\n"); abort(); }
/* Generated stub for fromwire_u32 */
u32 fromwire_u32(const u8 **cursor UNNEEDED, size_t *max UNNEEDED)
{ fprintf(stderr, "fromwire_u32 called!\n"); abort(); }
/* Generated stub for fromwire_u64 */
u64 fromwire_u64(const u8 **cursor UNNEEDED, size_t *max UNNEEDED)
{ fprintf(stderr, "fromwire_u64 called!\n"); abort(); }
/* Generated stub for fromwire_u8 */
u8 fromwire_u8(const u8 **cursor UNNEEDED, size_t *max UNNEEDED)
{ fprintf(stderr, "fromwire_u8 called!\n"); abort(); }
/* Generated stub for fromwire_u8_array */
void fromwire_u8_array(const u8 **cursor UNNEEDED, size_t *max UNNEEDED, u8 *arr UNNEEDED, size_t num UNNEEDED)
{ fprintf(stderr, "fromwire_u8_array called!\n"); abort(); }
/* Generated stub for pseudorand_u64 */
uint64_t pseudorand_u64(void)
{ fprintf(stderr, "pseudorand_u64 called!\n"); abort(); }
/* Generated stub for towire */
void towire(u8 **pptr UNNEEDED, const void *data UNNEEDED, size_t len UNNEEDED)
{ fprintf(stderr, "towire called!\n"); abort(); }
/* Generated stub for towire_bool */
void towire_bool(u8 **pptr UNNEEDED, bool v UNNEEDED)
{ fprintf(stderr, "towire_bool called!\n"); abort(); }
/* Generated stub for towire_secp256k1_ecdsa_signature */
void towire_secp256k1_ecdsa_signature(u8 **pptr UNNEEDED,
			      const secp256k1_ecdsa_signature *signature UNNEEDED)
{ fprintf(stderr, "towire_secp256k1_ecdsa_signature called!\n"); abort(); }
/* Generated stub for towire_sha256 */
void towire_sha256(u8 **pptr UNNEEDED, const struct sha256 *sha256 UNNEEDED)
{ fprintf(stderr, "towire_sha256 called!\n"); abort(); }
/* Generated stub for towire_u16 */
void towire_u16(u8 **pptr UNNEEDED, u16 v UNNEEDED)
{ fprintf(stderr, "towire_u16 called!\n"); abort(); }
/* Generated stub for towire_u32 */
void towire_u32(u8 **pptr UNNEEDED, u32 v UNNEEDED)
{ fprintf(stderr, "towire_u32 called!\n"); abort(); }
/* Generated stub for towire_u64 */
void towire_u64(u8 **pptr UNNEEDED, u64 v UNNEEDED)
{ fprintf(stderr, "towire_u64 called!\n"); abort(); }
/* Generated stub for towire_u8 */
void towire_u8(u8 **pptr UNNEEDED, u8 v UNNEEDED)
{ fprintf(stderr, "towire_u8 called!\n"); abort(); }
/* Generated stub for towire_u8_array */
void towire_u8_array(u8 **pptr UNNEEDED, const u8 *arr UNNEEDED, size_t num UNNEEDED)
{ fprintf(stderr, "towire_u8_array called!\n"); abort(); }
/* AUTOGENERATED MOCKS END */

static void diff_count(struct wally_psbt *a,
		       struct wally_psbt *b,
		       size_t diff_added,
		       size_t diff_rm)
{
	struct psbt_changeset *set;

	set = psbt_get_changeset(tmpctx, a, b);

	assert(tal_count(set->added_ins) == diff_added);
	assert(tal_count(set->added_outs) == diff_added);
	assert(tal_count(set->rm_ins) == diff_rm);
	assert(tal_count(set->rm_outs) == diff_rm);
}

static void add_in_out_with_serial(struct wally_psbt *psbt,
				   size_t serial_id,
				   size_t default_value)
{
	struct bitcoin_txid txid;
	u8 *script;
	struct amount_sat sat;
	struct wally_psbt_input *in;
	struct wally_psbt_output *out;

	memset(&txid, default_value, sizeof(txid));
	in = psbt_append_input(psbt, &txid, default_value, default_value,
			       NULL, NULL, NULL);
	if (!in)
		abort();
	psbt_input_set_serial_id(psbt, in, serial_id);

	script = tal_arr(tmpctx, u8, 20);
	memset(script, default_value, 20);
	sat = amount_sat(default_value);
	out = psbt_append_output(psbt, script, sat);
	if (!out)
		abort();
	psbt_output_set_serial_id(psbt, out, serial_id);
}

/* Try changing up the serial ids */
static void change_serials(void)
{
	struct wally_psbt *psbt;

	psbt = create_psbt(tmpctx, 1, 1, 0);
	add_in_out_with_serial(psbt, 10, 1);

	psbt_output_set_serial_id(psbt, &psbt->outputs[0], 2);
	assert(psbt_find_serial_output(psbt, 2) == 0);
	assert(psbt_find_serial_output(psbt, 10) == -1);

	psbt_input_set_serial_id(psbt, &psbt->inputs[0], 4);
	assert(psbt_find_serial_input(psbt, 4) == 0);
	assert(psbt_find_serial_input(psbt, 10) == -1);

}

static void check_psbt_comparison(void)
{
	struct wally_psbt *oldpsbt = psbt_from_b64(tmpctx, "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", strlen("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"));

	struct wally_psbt *newpsbt = psbt_from_b64(tmpctx, "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", strlen("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"));

	assert(!psbt_contribs_changed(oldpsbt, newpsbt));
}

int main(int argc, const char *argv[])
{
	common_setup(argv[0]);

	struct wally_psbt *start, *end;
	u32 flags = 0;

	chainparams = chainparams_for_network("bitcoin");

	/* Create two psbts! */
	end = create_psbt(tmpctx, 1, 1, 0);
	tal_wally_start();
	if (wally_psbt_clone_alloc(end, flags, &start) != WALLY_OK)
		abort();
	tal_wally_end(tmpctx);
	diff_count(start, end, 0, 0);
	diff_count(end, start, 0, 0);

	/* New input/output added */
	add_in_out_with_serial(end, 10, 1);
	diff_count(start, end, 1, 0);
	diff_count(end, start, 0, 1);

	/* Add another one, before previous */
	tal_wally_start();
	if (wally_psbt_clone_alloc(end, flags, &start) != WALLY_OK)
		abort();
	tal_wally_end(tmpctx);
	add_in_out_with_serial(end, 5, 2);
	diff_count(start, end, 1, 0);
	diff_count(end, start, 0, 1);

	/* Add another, at end */
	tal_wally_start();
	if (wally_psbt_clone_alloc(end, flags, &start) != WALLY_OK)
		abort();
	tal_wally_end(tmpctx);
	add_in_out_with_serial(end, 15, 3);
	diff_count(start, end, 1, 0);
	diff_count(end, start, 0, 1);

	/* Add another, in middle */
	tal_wally_start();
	if (wally_psbt_clone_alloc(end, flags, &start) != WALLY_OK)
		abort();
	tal_wally_end(tmpctx);
	add_in_out_with_serial(end, 11, 4);
	diff_count(start, end, 1, 0);
	diff_count(end, start, 0, 1);

	/* Change existing input/output info
	 * (we accomplish this by removing and then
	 * readding an input/output with the same serial_id
	 * but different value) */
	tal_wally_start();
	if (wally_psbt_clone_alloc(end, flags, &start) != WALLY_OK)
		abort();
	tal_wally_end(tmpctx);
	psbt_rm_output(end, 0);
	psbt_rm_input(end, 0);
	add_in_out_with_serial(end, 5, 5);
	diff_count(start, end, 1, 1);
	diff_count(end, start, 1, 1);

	/* Add some extra unknown info to a PSBT */
	u8 *key = psbt_make_key(tmpctx, 0x05, NULL);
	char *val = tal_fmt(tmpctx, "hello");
	psbt_input_set_unknown(end, &end->inputs[1], key, val, tal_bytelen(val));
	psbt_input_set_unknown(start, &start->inputs[1], key, val, tal_bytelen(val));

	/* Swap locations */
	struct wally_map_item tmp;
	tmp = end->inputs[1].unknowns.items[0];
	end->inputs[1].unknowns.items[0] = end->inputs[1].unknowns.items[1];
	end->inputs[1].unknowns.items[1] = tmp;

	/* We expect nothing to change ? */
	diff_count(start, end, 1, 1);
	diff_count(end, start, 1, 1);

	change_serials();

	check_psbt_comparison();

	/* No memory leaks please */
	common_shutdown();
	return 0;
}
