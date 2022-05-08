#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>

using namespace std;
#endif /* __PROGTEST__ */

/**
 * @brief Class implementing comparison between two dates.
 *
 * For comparing dates we should overload at least two operators(in our case "<" and "==") as we can evaluate the third
 * operator(">") just by using else statement.
 */
class CDate
{

public:
  // ~СDate
  /*
   * We do not need a destructor: When CDate object gets destructed, the default destructor is called.
   * This destructor calls destructor of every attribute of the class (int year, month, date).
   * Integers don`t have to be implicitly deallocated.
   */

  // parameterized constructor for initializing all attributes of the class
  CDate(int y, int m, int d) : year(y), month(m), day(d){};

  /**
   * @brief overloaded operator two compare contents of CDate classes
   *
   * @param[in] t object to compare
   * @returns  true if lvalue is lower than rvalue, or else if lvalue is bigger than rvalue
   */
  bool operator<(const CDate &t) const
  {
    return tie(year, month, day) < tie(t.year, t.month, t.day);
  }

  // same logic as in the overloaded operator before
  bool operator==(const CDate &t) const
  {
    return tie(year, month, day) == tie(t.year, t.month, t.day);
  }

private:
  int year, month, day;
};

/**
 * Main class which implements storing, selling and finding expired products. The most convenient container for doing
 * these kinds of operations is unordered map with set as value, as it allows to have constant time complexity for all
 * basic operations. Our class has a initializing constructor for our map and doesn`t have a destructor, as a default would
 * be enough. As you can notice, in private members we have a struct CProduct, which is responsible for a better storage
 * of product variables(expiry date and count in our case), and struct set_cmp, which is basically a comparator function
 * for our set(value) in unordered_map.
 */
class CSupermarket
{

public:
  // initializing constructor for our unordered_map.
  CSupermarket() : product_list(){};
  CSupermarket &store(const string &name, const CDate &expiry_date, int count);
  friend bool getRealName(CSupermarket &obj, string &product_validate);
  list<pair<string, int>> sell(list<pair<string, int>> &shoppingList);
  list<pair<string, int>> expired(const CDate &date) const;

private:
  /** @brief struct for convenient storage of the date and product count. Used in set in unordered_map product_list */
  struct CProduct
  {
    CDate expire_date;
    mutable int product_count;
  };

  /** @brief struct for custom set comparator. Compares two dates */
  struct set_cmp // code from stack_overflow for a custom set comparator
  {
    bool operator()(const CProduct &a, const CProduct &b) const
    {
      return a.expire_date < b.expire_date;
    }
  };

  unordered_map<string, set<CProduct, set_cmp>> product_list;
};

/** @brief comparator function for pairs based on second value. Used for sorting pairs in function "expiry" */
bool pairComparator(const pair<string, int> &x, const pair<string, int> &y)
{
  return (y.second < x.second);
}
/**
 * @brief Helper function for finding products with one mistake in the spelling.
 * @param[in] obj gaining access to private members.
 * @param[in,out] product_validate product name to find and change if we found only one matching word
 *
 * In this method we receive name of the product we want to potentially find in our unordered_map.
 * Time complexity of this function is O(n). Firstly, we have to iterate through every element, check if size matches and
 * then run an algorithm which compares individual letters and remembers how many mistakes were done before.
 * Consequently, if count of mistakes exceeds one, we go to another word and so on. Secondly, if ONLY
 * one word with single mistake was found, we make our product_validate equal to the name of the found products,
 * otherwise we return false.
 *
 * @returns true if only one name with one mistake was found, otherwise returns false.
 */
bool getRealName(CSupermarket &obj, string &product_validate)
{
  string temp_product;                             // string for storing found word
  int correct_words_cnt = 0;                       // count of words found with a single mistake
  for (const auto &map_element : obj.product_list) // iterating through map elements
  {
    if (map_element.first.size() != product_validate.size()) // size should be equal for comparing words
      continue;
    int mistake_counter = 0;
    for (size_t letter = 0; letter < map_element.first.size(); letter++) // iterating through letters of names

      if (map_element.first[letter] != product_validate[letter])
      {
        mistake_counter++;
        if (mistake_counter > 1)
          break;
      }

    if (mistake_counter == 1) // if the word has only one mistake we remember it and continue on with other words
    {
      correct_words_cnt++;
      if (correct_words_cnt > 1)
        return false;
      temp_product = map_element.first;
    }
  }
  if (correct_words_cnt == 1) // found the word to return
  {
    product_validate = temp_product;
    return true;
  }
  else
    return false;
}
/**
 * @brief Sells products listed in the shopping list
 *
 * @param[in,out] shoppingList list of all products we have to sell
 *
 *  Firstly, we check if product to sell is present in our products_list(unordered_map), this operation takes O(n)
 *  as we should account for a mistake in spelling of a product`s name. If we found the product, we iterate through
 *  it`s set and subtract the count of products beginning from the oldest(with lowest expiry date). Secondly, we
 *  should check whether we have deleted all structs from the set, in this case we delete a whole map element with
 *  the corresponding name. Structs from the set are deleted after each iteration through
 *  shoppingList(this is the most efficient way to do it). Sold items and map element are deleted after the whole
 *  iteration of shoppingList.
 *
 * @returns shoppingList without sold products
 */
list<pair<string, int>> CSupermarket::sell(list<pair<string, int>> &shoppingList)
{
  vector<list<pair<string, int>>::iterator> pair_to_delete;                              // vector to store pairs to delete from shoppingList (list)
  vector<unordered_map<string, set<CProduct, set_cmp>>::iterator> map_element_to_delete; // vector to store map_elements to delete from product_list  (map)

  for (auto product_buy = shoppingList.begin(); product_buy != shoppingList.end(); ++product_buy) // iterating through shopping list
  {
    if (product_list.count(product_buy->first) != 0 || getRealName(*this, product_buy->first)) // checking if element exists
    {
      auto const &product = product_list.find(product_buy->first); // iterator to the element in the map with same name as we are looking for
      vector<set<CProduct, set_cmp>::iterator> struct_to_delete;   // vector to store structs to delete from product_list.second  (set)

      for (auto set_it = product->second.begin(); set_it != product->second.end(); ++set_it) // iterating through set of structs
      {
        if (set_it->product_count < product_buy->second)
        {
          product_buy->second = product_buy->second - set_it->product_count;
          struct_to_delete.push_back(set_it);
          continue; // intentional continue
        }
        else if (set_it->product_count == product_buy->second)
        {
          pair_to_delete.push_back(product_buy);
          struct_to_delete.push_back(set_it);
          break; // intentional break to iterate through shopping list
        }
        else
        {
          set_it->product_count = set_it->product_count - product_buy->second;
          pair_to_delete.push_back(product_buy);
          break; // intentional break to iterate through shopping list
        }
      }
      for (auto &t : struct_to_delete) // clearing memory allocated for empty structs in the product_list.second  (set)
      {
        product_list[product->first].erase(t);
        if (product_list[product->first].size() == 0  ) // if value is empty - add the corresponding element to the "cleaning" vector
          map_element_to_delete.push_back(product);
      }
    }
  }
  for (auto &t : pair_to_delete) // clearing memory allocated for empty products in shopping list  (list)
  {
    shoppingList.erase(t);
  }
  for (auto &t : map_element_to_delete) // clearing memory allocated for empty products in the product_list  (map)
  {
    product_list.erase(t);
  }

  return shoppingList;
}

/** @brief Stores products in the map.
 *
 * @param[in] name is used as a key in unordered_map.
 * @param[in] expiry_date is added to set of structs.
 * @param[in] count is added to set of structs.
 *
 * In this method we store products as strings(key) and their expiry date and count as a set of structs(value).
 * This is the best way to do it, as it takes O(1) on average for all operations.
 * Firstly, we check if product we want to add is already in the unordered_map. Secondly, we check if the
 * expiry date of the product to add matches one of the dates in a set of structs. In case we found one, just
 * add the count to the product_count field. If there are no instances found with same expiry date, then we just
 * create another struct in the set with fields initialized as function parameters. Finally, if we didn`t find
 * any products with matching name, then we insert it as a new element in unordered_map.
 *
 * @returns current object with newly stored objects
 */
CSupermarket &CSupermarket::store(const string &name, const CDate &expiry_date, const int count)
{
  if (product_list.count(name) == 0)
  {
    set<CSupermarket::CProduct, set_cmp> to_ins = {CProduct{expiry_date, count}};
    product_list.insert(make_pair(name, to_ins)); // create new map element
  }
  else
  {
    auto const &product = product_list.find(name); // iterator to the element in map with same name

    for (auto set_it : product->second) // iterating through set of structs
    {
      if (set_it.expire_date == expiry_date)
      {
        set_it.product_count += count;
        return *this;
      }
    }                                                        // intentional dropdown
    product_list[name].insert(CProduct{expiry_date, count}); // create new struct in the set
  }
  return *this;
}

/**
 * @brief Creating a list of products which have expired before the date in the parameters of function call
 *
 * @param[in] date date we are comparing with
 *
 * Time complexity is O(n). Firstly, we iterate through every element of the map. Secondly, in each element we iterate
 * through set of structs and compare date(function parameter) with expiry_date of the individual product. If item
 * has been already expired, then we add its name and count of the products as a pair to the list we want to return.
 * Finally, after all iterations are done, we sort the resulting list in descending order by count, it takes O(nlogn).
 *
 * @returns list of expired products
 */
list<pair<string, int>> CSupermarket::expired(const CDate &date) const
{
  list<pair<string, int>> expired_products;
  for (auto const &product : product_list) // iterating through map
  {
    pair<string, int> product_pair{product.first, 0}; // creating pair to push at the back
    for (auto set_it : product.second)                // iterating through set
    {
      if (set_it.expire_date < date)
        product_pair.second += set_it.product_count;
    }
    if (product_pair.second != 0)
      expired_products.push_back(product_pair); // adding a new pair to the list
  }
  expired_products.sort(pairComparator);
  return expired_products;
}
#ifndef __PROGTEST__
int main(void)
{
  CSupermarket s;
  s.store("bread", CDate(2016, 4, 30), 100)
      .store("butter", CDate(2016, 5, 10), 10)
      .store("beer", CDate(2016, 8, 10), 50)
      .store("bread", CDate(2016, 4, 25), 100)
      .store("okey", CDate(2016, 7, 18), 5);

  list<pair<string, int>>
      l0 = s.expired(CDate(2018, 4, 30));
  assert(l0.size() == 4);
  assert((l0 == list<pair<string, int>>{{"bread", 200},
                                        {"beer", 50},
                                        {"butter", 10},
                                        {"okey", 5}}));

  list<pair<string, int>> l1{{"bread", 2},
                             {"Coke", 5},
                             {"butter", 20}};
  s.sell(l1);
  assert(l1.size() == 2);
  assert((l1 == list<pair<string, int>>{{"Coke", 5},
                                        {"butter", 10}}));

  list<pair<string, int>> l2 = s.expired(CDate(2016, 4, 30));

  assert(l2.size() == 1);
  assert((l2 == list<pair<string, int>>{{"bread", 98}}));

  list<pair<string, int>> l3 = s.expired(CDate(2016, 5, 20));
  assert(l3.size() == 1);
  assert((l3 == list<pair<string, int>>{{"bread", 198}}));

  list<pair<string, int>> l4{{"bread", 105}};
  s.sell(l4);
  assert(l4.size() == 0);
  assert((l4 == list<pair<string, int>>{}));

  list<pair<string, int>> l5 = s.expired(CDate(2017, 1, 1));
  assert(l5.size() == 3);
  assert((l5 == list<pair<string, int>>{{"bread", 93},
                                        {"beer", 50},
                                        {"okey", 5}}));

  s.store("Coke", CDate(2016, 12, 31), 10);

  list<pair<string, int>> l6{{"Cake", 1},
                             {"Coke", 1},
                             {"cake", 1},
                             {"coke", 1},
                             {"cuke", 1},
                             {"Cokes", 1}};
  s.sell(l6);
  assert(l6.size() == 3);
  assert((l6 == list<pair<string, int>>{{"cake", 1},
                                        {"cuke", 1},
                                        {"Cokes", 1}}));

  list<pair<string, int>> l7 = s.expired(CDate(2017, 1, 1));
  assert(l7.size() == 4);
  assert((l7 == list<pair<string, int>>{{"bread", 93},
                                        {"beer", 50},
                                        {"Coke", 7},
                                        {"okey", 5}}));

  s.store("cake", CDate(2016, 11, 1), 5);

  list<pair<string, int>> l8{{"Cake", 1},
                             {"Coke", 1},
                             {"cake", 1},
                             {"coke", 1},
                             {"cuke", 1}};
  s.sell(l8);
  assert(l8.size() == 2);
  assert((l8 == list<pair<string, int>>{{"Cake", 1},
                                        {"coke", 1}}));

  list<pair<string, int>> l9 = s.expired(CDate(2017, 1, 1));
  assert(l9.size() == 5);
  assert((l9 == list<pair<string, int>>{{"bread", 93},
                                        {"beer", 50},
                                        {"Coke", 6},
                                        {"okey", 5},
                                        {"cake", 3}}));

  list<pair<string, int>> l10{{"cake", 15},
                              {"Cake", 2}};
  s.sell(l10);
  assert(l10.size() == 2);
  assert((l10 == list<pair<string, int>>{{"cake", 12},
                                         {"Cake", 2}}));

  list<pair<string, int>> l11 = s.expired(CDate(2017, 1, 1));
  assert(l11.size() == 4);
  assert((l11 == list<pair<string, int>>{{"bread", 93},
                                         {"beer", 50},
                                         {"Coke", 6},
                                         {"okey", 5}}));

  list<pair<string, int>> l12{{"Cake", 4}};
  s.sell(l12);
  assert(l12.size() == 0);
  assert((l12 == list<pair<string, int>>{}));

  list<pair<string, int>> l13 = s.expired(CDate(2017, 1, 1));
  assert(l13.size() == 4);
  assert((l13 == list<pair<string, int>>{{"bread", 93},
                                         {"beer", 50},
                                         {"okey", 5},
                                         {"Coke", 2}}));

  list<pair<string, int>> l14{{"Beer", 20},
                              {"Coke", 1},
                              {"bear", 25},
                              {"beer", 10}};
  s.sell(l14);
  assert(l14.size() == 1);
  assert((l14 == list<pair<string, int>>{{"beer", 5}}));

  s.store("ccccb", CDate(2019, 3, 11), 100)
      .store("ccccd", CDate(2019, 6, 9), 100)
      .store("dcccc", CDate(2019, 2, 14), 100);

  list<pair<string, int>> l15{{"ccccc", 10}};
  s.sell(l15);
  assert(l15.size() == 1);
  assert((l15 == list<pair<string, int>>{{"ccccc", 10}}));

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
