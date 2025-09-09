#ifndef PIXELPART_SORT_H
#define PIXELPART_SORT_H

#include <algorithm>

namespace godot {
template <typename It, typename Comp = std::less<typename std::iterator_traits<It>::value_type>>
void insertion_sort(It begin, It end, Comp comp = Comp()) {
	std::iter_swap(begin, std::min_element(begin, end, comp));

	for(It i = begin; ++i < end; begin = i) {
		for(It j = i; comp(*j, *begin); --j, --begin) {
			std::iter_swap(begin, j);
		}
	}
}
}

#endif
