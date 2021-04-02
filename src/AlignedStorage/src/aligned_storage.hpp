/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   aligned_storage.hpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/02 17:31:57 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/02 17:34:59 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALIGNED_STORAGE_HPP
#define ALIGNED_STORAGE_HPP

#include "../../Traits/src/type_traits.hpp"

namespace Utils {

template <typename T> class aligned_storage {
    union dummy_u {
        char data[sizeof(T)];
        typedef typename meta::type_with_alignment<meta::alignment_of<T>::value>::type aligner;
    } dummy;

  public:
    void const* address(void) const { return &dummy.data[0]; }
    void* address(void) { return &dummy.data[0]; }
};

} // namespace Utils

#endif
