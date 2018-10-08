#ifndef CPP_ESSENTIALS_SQ_SQ_HPP_
#define CPP_ESSENTIALS_SQ_SQ_HPP_

#pragma once

#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/core/algorithm_ext.hpp>
#include <cpp_essentials/core/backwards.hpp>
#include <cpp_essentials/core/chain.hpp>
#include <cpp_essentials/core/chunk.hpp>
#include <cpp_essentials/core/containers.hpp>
#include <cpp_essentials/core/elementwise.hpp>
#include <cpp_essentials/core/filter.hpp>
#include <cpp_essentials/core/flatten.hpp>
#include <cpp_essentials/core/index.hpp>
#include <cpp_essentials/core/iterate.hpp>
#include <cpp_essentials/core/join.hpp>
#include <cpp_essentials/core/map.hpp>
#include <cpp_essentials/core/numeric.hpp>
#include <cpp_essentials/core/slice.hpp>
#include <cpp_essentials/core/stride.hpp>
#include <cpp_essentials/core/trim.hpp>
#include <cpp_essentials/core/zip.hpp>

namespace cpp_essentials::sq
{

#define PULL_ADAPTABLE(func) \
    static constexpr core::adaptable<decltype(core::func)> func

#define PULL_FUNCTOR(func) \
    static constexpr decltype(core::func) func

PULL_FUNCTOR(range);
PULL_FUNCTOR(inclusive_range);
PULL_FUNCTOR(infinite_range);

PULL_ADAPTABLE(front);
PULL_ADAPTABLE(front_or);
PULL_ADAPTABLE(front_or_default);
PULL_ADAPTABLE(front_or_eval);
PULL_ADAPTABLE(front_or_none);
PULL_ADAPTABLE(single);
PULL_ADAPTABLE(single_or);
PULL_ADAPTABLE(single_or_default);
PULL_ADAPTABLE(single_or_eval);
PULL_ADAPTABLE(single_or_none);
PULL_ADAPTABLE(size);
PULL_ADAPTABLE(empty);
PULL_ADAPTABLE(non_empty);
PULL_ADAPTABLE(copy_while);
PULL_ADAPTABLE(copy_until);
PULL_ADAPTABLE(starts_with);
PULL_ADAPTABLE(ends_with);
PULL_ADAPTABLE(contains);

PULL_ADAPTABLE(accumulate);
PULL_ADAPTABLE(all_of);
PULL_ADAPTABLE(any_of);
PULL_ADAPTABLE(copy);
PULL_ADAPTABLE(copy_if);
PULL_ADAPTABLE(copy_n);
PULL_ADAPTABLE(count);
PULL_ADAPTABLE(count_if);
PULL_ADAPTABLE(for_each);
PULL_ADAPTABLE(is_heap);
PULL_ADAPTABLE(is_partitioned);
PULL_ADAPTABLE(is_permutation);
PULL_ADAPTABLE(is_sorted);
PULL_ADAPTABLE(lower_bound);
PULL_ADAPTABLE(max_element);
PULL_ADAPTABLE(minmax_element);
PULL_ADAPTABLE(min_element);
PULL_ADAPTABLE(move);
PULL_ADAPTABLE(none_of);
PULL_ADAPTABLE(upper_bound);

PULL_ADAPTABLE(backwards);

PULL_ADAPTABLE(chain);
PULL_ADAPTABLE(append);
PULL_ADAPTABLE(prepend);

PULL_ADAPTABLE(chunk);
PULL_ADAPTABLE(slide);
PULL_ADAPTABLE(group);
PULL_ADAPTABLE(pairwise);

PULL_ADAPTABLE(to_vector);
PULL_ADAPTABLE(to_set);
PULL_ADAPTABLE(to_list);
PULL_ADAPTABLE(to_forward_list);
PULL_ADAPTABLE(to_deque);

PULL_ADAPTABLE(take_if);
PULL_ADAPTABLE(drop_if);

PULL_ADAPTABLE(flatten);

PULL_ADAPTABLE(index);

PULL_ADAPTABLE(iterate);

PULL_ADAPTABLE(join);

PULL_ADAPTABLE(map);

PULL_ADAPTABLE(slice);
PULL_ADAPTABLE(take);
PULL_ADAPTABLE(drop);
PULL_ADAPTABLE(take_back);
PULL_ADAPTABLE(drop_back);
PULL_ADAPTABLE(py_slice);

PULL_ADAPTABLE(stride);

PULL_ADAPTABLE(take_while);
PULL_ADAPTABLE(drop_while);
PULL_ADAPTABLE(take_until);
PULL_ADAPTABLE(drop_until);
PULL_ADAPTABLE(take_back_while);
PULL_ADAPTABLE(drop_back_while);
PULL_ADAPTABLE(take_back_until);
PULL_ADAPTABLE(drop_back_until);
PULL_ADAPTABLE(trim_while);
PULL_ADAPTABLE(trim_until);

PULL_ADAPTABLE(zip);

#undef PULL_ADAPTABLE
#undef PULL_FUNCTOR

} /* namespace sq */

#endif /* CPP_ESSENTIALS_SQ_SQ_HPP_ */