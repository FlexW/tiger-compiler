/**
 * @file check_linked_list.c
 * Unit test for linked list modul.
 */
#include <stdlib.h>
#include <stdio.h>
#include <check.h>

#include "../src/include/linked_list.h"

START_TEST (test_linked_list_get_three_elements)
{
  char         element1[]   = "element1";
  char         element2[]   = "element2";
  char         element3[]   = "element3";
  linked_list *list         = linked_list_new ();

  linked_list_add (list, element1);
  linked_list_add (list, element2);
  linked_list_add (list, element3);

  char *str;

  str = linked_list_get (list, 0);
  ck_assert_str_eq (str, element1);

  str = linked_list_get (list, 1);
  ck_assert_str_eq (str, element2);

  str = linked_list_get (list, 2);
  ck_assert_str_eq (str, element3);

  linked_list_free (list);
}
END_TEST

START_TEST (test_linked_list_remove_last)
{
  char         element1[] = "element1";
  char         element2[] = "element2";
  char         element3[] = "element3";
  linked_list *list       = linked_list_new ();

  linked_list_add (list, element1);
  linked_list_add (list, element2);
  linked_list_add (list, element3);

  bool res = linked_list_remove (list, linked_list_length (list) - 1);
  if (res == false)
    ck_abort_msg ("res == false. Element got not removed.");

  char *str = linked_list_get (list, linked_list_length (list) - 1);
  if (str == NULL)
    ck_abort_msg ("str == NULL. Element not found.");
  ck_assert_str_eq (element2, str);

  //linked_list_free (list);
}
END_TEST

START_TEST (test_linked_list_remove_first)
{
  char         element1[]   = "element1";
  char         element2[]   = "element2";
  char         element3[]   = "element3";
  linked_list *list         = linked_list_new ();

  linked_list_add (list, element1);
  linked_list_add (list, element2);
  linked_list_add (list, element3);

  bool res = linked_list_remove (list, 0);
  if (res == false)
    ck_abort_msg ("res == false. Element got not removed.");

  char * str = linked_list_get (list, 0);
  if (str == NULL)
    ck_abort_msg ("str == NULL. Element not found.");
  ck_assert_str_eq (element2, str);

  linked_list_free (list);
}
END_TEST

START_TEST (test_linked_list_remove_middle)
{
  char *str;
  char  element1[] = "element1";
  char  element2[] = "element2";
  char  element3[] = "element3";

  linked_list *list = linked_list_new ();

  linked_list_add (list, element1);
  linked_list_add (list, element2);
  linked_list_add (list, element3);

  linked_list_remove (list, 1);

  str = linked_list_get (list, 0);
  ck_assert_str_eq (str, element1);

  str = linked_list_get (list, 1);
  ck_assert_str_eq (str, element3);

  linked_list_free (list);
}
END_TEST

START_TEST (test_linked_list_remove_out_range)
{
  char *str;
  char  element1[] = "element1";
  char  element2[] = "element2";
  char  element3[] = "element3";

  linked_list *list = linked_list_new ();

  linked_list_add (list, element1);
  linked_list_add (list, element2);
  linked_list_add (list, element3);

  linked_list_remove (list, -1);
  linked_list_remove (list, 3);

  str = linked_list_get (list, 0);
  ck_assert_str_eq (str, element1);

  str = linked_list_get (list, 1);
  ck_assert_str_eq (str, element2);

  str = linked_list_get (list, 2);
  ck_assert_str_eq (str, element3);

  int len = linked_list_length (list);
  ck_assert_int_eq (len, 3);

  linked_list_free (list);
}
END_TEST

START_TEST (test_linked_list_insert_middle)
{
  char *str;
  char element1[] = "element1";
  char element2[] = "element2";
  char element3[] = "element3";
  char element4[] = "element4";

  linked_list *list = linked_list_new ();

  linked_list_add (list, element1);
  linked_list_add (list, element2);
  linked_list_add (list, element3);

  linked_list_insert (list, 1, element4);

  str     = linked_list_get (list, 1);
  int len = linked_list_length (list);

  ck_assert_str_eq (str, element4);
  ck_assert_int_eq (len, 4);

  linked_list_free (list);
}
END_TEST

START_TEST (test_linked_list_insert_out_range)
{
  char *str;
  char element1[] = "element1";
  char element2[] = "element2";
  char element3[] = "element3";
  char element4[] = "element4";

  linked_list *list = linked_list_new ();

  linked_list_add (list, element1);
  linked_list_add (list, element2);
  linked_list_add (list, element3);

  linked_list_insert (list, -1, element4);
  linked_list_insert (list, 3, element4);

  str     = linked_list_get (list, 1);
  int len = linked_list_length (list);

  ck_assert_str_eq (str, element2);
  ck_assert_int_eq (len, 3);

  linked_list_free (list);
}
END_TEST

START_TEST (test_linked_list_free)
{
  char element1[] = "element1";
  char element2[] = "element2";
  char element3[] = "element3";

  linked_list *list = linked_list_new ();

  linked_list_add (list, element1);
  linked_list_add (list, element2);
  linked_list_add (list, element3);

  int res = linked_list_free (list);

  ck_assert_int_eq (res, 3);
}
END_TEST

START_TEST (test_linked_list_for_each)
{
  char *str;
  char element1[] = "element1";
  char element2[] = "element2";
  char element3[] = "element3";

  linked_list *list = linked_list_new ();

  linked_list_add (list, element1);
  linked_list_add (list, element2);
  linked_list_add (list, element3);

  char *element;
  LINKED_LIST_FOR_EACH (element, list)
    element[0] = 'c';

  str = linked_list_get (list, 0);
  ck_assert_str_eq (str, "clement1");

  str = linked_list_get (list, 1);
  ck_assert_str_eq (str, "clement2");

  str = linked_list_get (list, 2);
  ck_assert_str_eq (str, "clement3");

  linked_list_free (list);
}
END_TEST

Suite *
linked_list_suite (void)
{
  Suite *s = suite_create ("linked_list");

  TCase *tc_core = tcase_create ("Core");

  tcase_add_test (tc_core, test_linked_list_get_three_elements);
  tcase_add_test (tc_core, test_linked_list_remove_last);
  tcase_add_test (tc_core, test_linked_list_remove_middle);
  tcase_add_test (tc_core, test_linked_list_remove_first);
  tcase_add_test (tc_core, test_linked_list_remove_out_range);
  tcase_add_test (tc_core, test_linked_list_insert_middle);
  tcase_add_test (tc_core, test_linked_list_insert_out_range);
  tcase_add_test (tc_core, test_linked_list_free);
  tcase_add_test (tc_core, test_linked_list_for_each);

  suite_add_tcase(s, tc_core);

  return s;
}

int
main (void)
{
  Suite   *s  = linked_list_suite ();
  SRunner *sr = srunner_create (s);

  srunner_run_all (sr, CK_NORMAL);
  int number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
