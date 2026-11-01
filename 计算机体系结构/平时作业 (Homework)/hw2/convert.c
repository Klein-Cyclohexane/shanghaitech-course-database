#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    uint8_t sign;
    uint32_t exponent;
    uint32_t mantissa;
} ca25;

typedef struct {
    uint32_t sign : 1;
    uint32_t exponent : 8;
    uint32_t mantissa : 23;
} fp32;

ca25 parse_ca25(const char *hex_str) {
    ca25 num;
    uint64_t hex_value = 0;
    
    for (int i = 0; i < 16; i++) {
        char c = hex_str[i];
        uint8_t digit;
        
        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else if (c >= 'a' && c <= 'f') {
            digit = c - 'a' + 10;
        } else {
            digit = 0;
        }
        
        hex_value = (hex_value << 4) | digit;
    }
    
    num.sign = (hex_value >> 63) & 0x1;
    num.exponent = (hex_value >> 31) & 0xFFFFFFFF;
    num.mantissa = hex_value & 0x7FFFFFFF;
    
    return num;
}

fp32 convert_to_fp32(ca25 num) {
    fp32 result = {0}; 
    result.sign = num.sign;
    
    if (num.exponent == 0xFFFFFFFF) {
        result.exponent = 0xFF;
        if (num.mantissa != 0) {
            result.mantissa = (num.mantissa >> 8) & 0x7FFFFF;
            if (result.mantissa == 0) {
                result.mantissa = 1;
            }
        }
        return result;
    }
    
    if (num.exponent == 0 && num.mantissa == 0) {
        return result;
    }
    
    int is_normal_ca25 = (num.exponent != 0);
    
    int64_t unbiased_exp;
    if (is_normal_ca25) {
        unbiased_exp = (int64_t)num.exponent - 0x7FFFFFFF;
    } else {
        unbiased_exp = 1 - 0x7FFFFFFF;
        uint32_t m = num.mantissa;
        while ((m & 0x40000000) == 0 && m != 0) {
            m <<= 1;
            unbiased_exp--;
        }
        if (m == 0) {
            return result;
        }
    }
    
    int64_t fp32_biased_exp = unbiased_exp + 127;
    uint64_t significand;
    if (is_normal_ca25) {
        significand = ((uint64_t)1 << 31) | num.mantissa;
    } else {
        significand = ((uint64_t)1 << 31) | (num.mantissa & 0x3FFFFFFF);
    }
    
    if (fp32_biased_exp >= 0xFF) {
        result.exponent = 0xFF;
        result.mantissa = 0;
        return result;
    } else if (fp32_biased_exp <= 0) {
        int shift_amount = 1 - fp32_biased_exp + (31 - 23);
        
        if (shift_amount >= 32) {
            if (significand != 0) {
                result.exponent = 0;
                result.mantissa = 1;
            }
            return result;
        }
        
        uint64_t round_bits = significand & ((1ULL << shift_amount) - 1);
        significand >>= shift_amount;
        
        if (round_bits != 0) {
            significand += 1;
        }
        if (significand >= (1ULL << 23)) {
            result.exponent = 1;
            significand = 0;
        } else {
            result.exponent = 0;
            result.mantissa = significand & 0x7FFFFF;
        }
        
        return result;
    } else {
        int shift_amount = 8; // 31 - 23

        uint64_t round_bits = significand & ((1ULL << shift_amount) - 1);
        significand >>= shift_amount;
        
        if (round_bits != 0) {
            significand += 1;
            
            if (significand >= (1ULL << 24)) {
                significand >>= 1;
                fp32_biased_exp += 1;
                if (fp32_biased_exp >= 0xFF) {
                    result.exponent = 0xFF;
                    result.mantissa = 0; 
                    return result;
                }
            }
        }
        
        result.exponent = fp32_biased_exp;
        result.mantissa = significand & 0x7FFFFF;
        return result;
    }
}

const char* get_type(uint8_t is_ca25, uint32_t exponent, uint32_t mantissa) {
    if (is_ca25) {
        if (exponent == 0xFFFFFFFF) {
            return mantissa == 0 ? "inf" : "nan";
        } else if (exponent == 0) {
            return mantissa == 0 ? "zero" : "subnormal";
        } else {
            return "normal";
        }
    } else {
        if (exponent == 0xFF) {
            return mantissa == 0 ? "inf" : "nan";
        } else if (exponent == 0) {
            return mantissa == 0 ? "zero" : "subnormal";
        } else {
            return "normal";
        }
    }
}

void print_ca25_info(ca25 num) {
    const char* type = get_type(1, num.exponent, num.mantissa);
    int implicit_bit = (num.exponent != 0 && strcmp(type, "zero") != 0) ? 1 : 0;
    
    printf("ca25 S=%d E=%08x M=%d.%08x %s\n", 
           num.sign, 
           num.exponent,
           implicit_bit, 
           (num.mantissa << 1) & 0xFFFFFFFE,
           type);
}

void print_fp32_info(fp32 num) {
    const char* type = get_type(0, num.exponent, num.mantissa);
    int implicit_bit = (num.exponent != 0 && strcmp(type, "zero") != 0) ? 1 : 0;
    
    printf("fp32 S=%d E=%02x M=%d.%06x %s\n", 
           num.sign, 
           num.exponent,
           implicit_bit, 
           (num.mantissa << 1) & 0xFFFFFF, 
           type);
}

void print_fp32_hex(fp32 num) {
    uint32_t hex_value = (num.sign << 31) | (num.exponent << 23) | num.mantissa;
    printf("%08x\n", hex_value);
}

int main(void) {
    char hex_str[17];

    while (scanf("%16s", hex_str) == 1) {
        ca25 num = parse_ca25(hex_str);
        print_ca25_info(num);
        fp32 converted = convert_to_fp32(num);
        print_fp32_info(converted);
        print_fp32_hex(converted);
    }
    return 0;
}
