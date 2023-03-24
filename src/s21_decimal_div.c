//
//  s21_decimal_arifmetic.c
//  test
//
//  Created by mcbk on 15.12.2022.
//

#include <stdio.h>

#include "s21_decimal.h"
#include "supp_func.h"
// The functions return the error code:
//- 0 - OK
//- 1 - the number is too large or equal to infinity
//- 2 - the number is too small or equal to negative infinity
//- 3 - division by 0

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int err = 0;
  memset(result, 0, 16);
  if (is_dec_zero(value_2)) {
    err = 3;
  } else if (is_dec_zero(value_1)) {
    for (int i = 0; i < 3; ++i) result->bits[i] = 0;
    if (get_sign(value_2.bits[3]))
      // if (!get_sign(result->bits[3]))
      s21_negate(*result, result);
  } else {
    int exp = get_exp(value_1.bits[3]) - get_exp(value_2.bits[3]);
    s21_big_decimal temp1, temp2, temp_res, frac_part;
    memset(&temp_res, 0, 28);
    memset(&temp1, 0, 28);
    memset(&temp2, 0, 28);
    memset(&frac_part, 0, 28);
    for (int i = 0; i < 3; ++i) {
      temp1.bits[i] = value_1.bits[i];
      temp2.bits[i] = value_2.bits[i];
    }
    // printf("asdasd");
    //        print_bits_big_decimal(&temp1);
    //        print_bits_big_decimal(&temp2);
    int overflow_flag = 0, flag_finish = 0;
    while (exp < 96 && !overflow_flag && !flag_finish) {
      // print_bits_big_decimal(&temp_res);
      memset(&frac_part, 0, 28);
      if (s21_is_less_or_equal_big(temp2, temp1)) {
        temp1 = s21_div_supp(temp1, temp2, &frac_part);
        is_big_dec_zero(temp1) ? flag_finish = 1 : 1;
        sum_equal_exp(temp_res, frac_part, &temp_res, 100);
      } else {
        if (temp_res.bits[3] == 0) {
          _x10(&temp_res);
          ++exp;
          _x10(&temp1);
          temp1 = s21_div_supp(temp1, temp2, &frac_part);
          is_big_dec_zero(temp1) ? flag_finish = 1 : 1;
          sum_equal_exp(temp_res, frac_part, &temp_res, 100);
        } else {
          overflow_flag = 1;
        }
      }
    }

    // printf("lb1 = %d  lb2 =%d  \n", get_last_bit(temp_res),
    // get_last_bit(temp1));
    //    set_exp(exp, &(temp_res.bits[6]));
    if ((!get_sign(value_1.bits[3]) && get_sign(value_2.bits[3])) ||
        (get_sign(value_1.bits[3]) && !get_sign(value_2.bits[3])))
      s21_negate_big(temp_res, &temp_res);
    err = convert_big_to_dec(temp_res, result, exp, 1);
    // print_bits_big_decimal(&temp1);
    // print_bits_big_decimal(&temp_res);
    // printf("exp = %d\n", exp);
  }
  return err;
}

s21_big_decimal s21_div_supp(s21_big_decimal value_1, s21_big_decimal value_2,
                             s21_big_decimal *result) {
  memset(result, 0, 28);
  set_exp(0, &(value_1.bits[6]));
  set_exp(0, &(value_2.bits[6]));
  int lb1 = get_last_bit(value_1), lb2 = get_last_bit(value_2);
  int diff = lb1 - lb2;
  //  printf("lb1 = %d  lb2 =%d  d = %d\n", lb1, lb2, diff);
  for (int i = 0; i < diff; ++i) shuffle_big_dec_left(&value_2);
  //    print_bits_big_decimal(&value_1);
  //     print_bits_big_decimal(&value_2);
  //  printf(" \n");
  while (diff >= 0) {
    if (s21_is_less_or_equal_big(value_2, value_1)) {
      //            print_bits_big_decimal(&value_1);
      //            print_bits_big_decimal(&value_2);
      minus_equal_exp(value_1, value_2, &value_1);
      int bit = diff % 32;
      bit == 0 ? bit = diff : 1;
      //   print_bits_big_decimal(&value_2);
      set_bit(&(result->bits[diff / 32]), bit, 1);
      //   print_bits_big_decimal(&value_2);
      shuffle_big_dec_right(&value_2);
      //            print_bits_big_decimal(&value_1);
      //            print_bits_big_decimal(&value_2);
      // printf(" \n");
    } else {
      int bit = diff % 32;
      diff < 32 ? bit = diff : 1;
      set_bit(&(result->bits[diff / 32]), bit, 0);
      shuffle_big_dec_right(&value_2);
      //            print_bits_big_decimal(&value_1);
      //            print_bits_big_decimal(&value_2);
      //  printf(" \n");
    }
    //  printf(" bit = %d, i = %d\n", diff % 32, diff/32);
    --diff;
  }
  //    printf("res");
  //    print_bits_big_decimal(result);
  //    printf("\n");
  return value_1;
}

s21_big_decimal div10(s21_big_decimal *value_1) {
  s21_big_decimal value_2;
  memset(&value_2, 0, 28);
  value_2.bits[0] = 10;
  s21_big_decimal result;
  memset(&result, 0, 28);
  result = s21_div_supp(*value_1, value_2, value_1);
  return result;
}

// int check_div( s21_decimal v1, s21_decimal v2) {
//     int result = 0;
//
//     return result;
// }
