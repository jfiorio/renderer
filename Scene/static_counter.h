/*********************************************************************************
 * This template meta-program provides support for constant expression           *
 * (constexpr) compile-time counters.                                            *
 *                                                                               *
 * The idea behind it is really rather simple. It essentially uses a             *
 * combination of SFINAE, Overload Resolution, and CRTP. If we have the          *
 * ability to change compilation state and we have the ability gague that        *
 * change in compilation state, then we have the ability to incrementally        *
 * develop a value during compile time.                                          *
 *                                                                               *
 * See counter.txt for a detailed explanation.                                   *
 *                                                                               *
 * Here we choose M = 32. Thus, 32 functions and writers can be defined, and     *
 * the counter can have a maximum value of 32. We use a non-type template        *
 * argument that takes a default value of 1 for the "counter increment value"    *
 * -i.e. the 1 in instantiating the "C+1th" writer is now optionally variable.   *
 * The ambiguity of the reader overloads is acheived with the choice between     *
 * int and float for the first argument; this allows the first overload to be    *
 * the more viable choice for resolution in the case of no substitution failure. *
 *********************************************************************************/

#ifndef STATIC_COUNTER_H
#define STATIC_COUNTER_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-template-friend"

template<typename T, int ... id>                                                                                      
struct static_counter 
{
  template<int N>
  struct flag {
    friend constexpr int adl_flag (flag<N>);
  };
  template<int N>
  struct writer {
    friend constexpr int adl_flag (flag<N>) {
      return N;
    }
    static constexpr int value = N;
  };
  template<int N, int = adl_flag (flag<N> {})>
  static int constexpr reader (int, flag<N>) {
    return N;
  }
  template<int N>
  static int constexpr reader (float, flag<N>, int R = reader (0, flag<N-1> {})) {
    return R;
  }
  static int constexpr reader (float, flag<0>) {
    return 0;
  }
  template<int N = 1>
  static int constexpr next (int R = writer<reader (0, flag<32> {}) + N>::value) {
    return R;
  }
};

#pragma GCC diagnostic pop

#endif