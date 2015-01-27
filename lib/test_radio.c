#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

// include, we are testing static methods
#include "radio.c"


// the goal is just to make sure the function provides the same results
// after refactorization
static void test_frame_diff_dec(void **state)
{
    (void) state;   // unused

    uint8_t data_in[FRAME_LEN];
    uint8_t data_out[sizeof(data_in)];
    for (int i = 0; i < 8; ++i) {
        data_in[i] = data_out[i] = 1 << (i % 7);
    }
    for (int i = 0; i < FRAME_LEN - 8; ++i) {
        data_in[i + 8] = 1 << (i % 7);
    }
    uint8_t data_exp[FRAME_LEN] = {
        0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x01,
        0x01, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60, 0x21, 0x03, 0x06, 0x0a, 0x18,
        0x30, 0x50, 0x41, 0x03, 0x05, 0x0c, 0x18, 0x28, 0x60, 0x41, 0x42, 0x06,
        0x0c, 0x14, 0x30, 0x60, 0x21, 0x03, 0x06, 0x0a, 0x18, 0x30, 0x50, 0x41,
        0x03, 0x05, 0x0c, 0x18, 0x28, 0x60, 0x41, 0x42, 0x06, 0x0c, 0x14, 0x30,
        0x60, 0x21, 0x03, 0x06, 0x0a, 0x18, 0x30, 0x50, 0x41, 0x03, 0x05, 0x0c,
        0x18, 0x28, 0x60, 0x41, 0x42, 0x06, 0x0c, 0x14, 0x30, 0x60, 0x21, 0x03,
        0x06, 0x0a, 0x18, 0x30, 0x50, 0x41, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x50,
        0x41, 0x03, 0x05, 0x0c, 0x18, 0x28, 0x60, 0x41, 0x42, 0x06, 0x0c, 0x14,
        0x30, 0x60, 0x21, 0x03, 0x06, 0x0a, 0x18, 0x30, 0x50, 0x41, 0x03, 0x05,
        0x0c, 0x18, 0x28, 0x60, 0x41, 0x42, 0x06, 0x0c, 0x14, 0x30, 0x60, 0x21,
        0x03, 0x06, 0x0a, 0x18, 0x30, 0x50, 0x41, 0x03, 0x05, 0x0c, 0x18, 0x28,
        0x60, 0x41, 0x42, 0x06, 0x0c, 0x14, 0x30, 0x60, 0x21, 0x03, 0x06, 0x0a,
        0x18, 0x30, 0x50, 0x41, 0x03, 0x05, 0x0c, 0x18,
    };

    diffdec_frame(data_in + 8, data_out + 8, FRAME_LEN - 8);
    assert_memory_equal(data_exp, data_out, FRAME_LEN);
}

static void test_mk_crc5(void **state)
{
    (void) state;   // unused

    {
        const uint8_t in[] = { 1, 0, 1, 0, 1, 0 };
        const uint8_t out_exp[] = { 0, 1, 0, 1, 1 };
        uint8_t out[5];
        mk_crc5(out, in, sizeof(out_exp));
        assert_memory_equal(out, out_exp, sizeof(out_exp));
    }

    {
        const uint8_t in[] = { 0, 0, 0, 0, 0, 0 };
        const uint8_t out_exp[] = { 0, 0, 0, 0, 0 };
        uint8_t out[5];
        mk_crc5(out, in, sizeof(out_exp));
        assert_memory_equal(out, out_exp, sizeof(out_exp));
    }

    {
        const uint8_t in[] = { 1, 1, 1, 1, 1, 1 };
        const uint8_t out_exp[] = { 0, 1, 1, 0, 0 };
        uint8_t out[5];
        mk_crc5(out, in, sizeof(out_exp));
        assert_memory_equal(out, out_exp, sizeof(out_exp));
    }
}

int main(void)
{
    const UnitTest tests[] = {
        unit_test(test_frame_diff_dec),
        unit_test(test_mk_crc5),
    };

    return run_tests(tests);
}
