/** @file CommaSeparatedIterator.cpp
 * Implementation of class to read comma-delimited input text streams.
*/
#include "CommaSeparatedIterator.h"

#include "Driver.h"

#include <cstddef>    // for std::size_t
#include <istream>    // for std::istream
#include <stdexcept>  // for std::runtime_error
#include <string>     // for std::getline, std::string

CommaSeparatedIterator::CommaSeparatedIterator(std::istream &stream):
    stream_(stream) {
    ++(*this);  // Move to the first line
}

/***********************************************************************
 * Pre-increment operator.
 * 
 * Advances the iterator to the next line and splits it into two substrings.
 * If the end of the stream is reached, both substrings will be empty. Both
 * parsed substrings are converted to lowercase.
 * 
 * @return A reference to the updated iterator.
 **********************************************************************/
CommaSeparatedIterator &CommaSeparatedIterator::operator++() {
    if (std::getline(stream_, line_)) {
        // Skip line if empty
        if (line_.empty()) {
            return ++(*this);
        }

        // Parse line by comma delimiter
        std::size_t pos = line_.find(',');
        if (pos != std::string::npos) {
            first_ = line_.substr(0, pos);
            second_ = line_.substr(pos + 1);
        } else {
            first_ = line_;
            second_ = "";
        }

        // Convert both substrings to lowercase
        StringToLower(first_);
        StringToLower(second_);
    } else {
        if (stream_.bad()) {
            throw std::runtime_error("Error reading stream.");
        }
        // End of stream reached
        first_ = second_ = "";
    }

    return *this;
}

/***********************************************************************
 * Dereference operator.
 * 
 * Returns the current pair of substrings (first and second).
 * 
 * @return A pair containing the two substrings from the current line.
 **********************************************************************/
CommaSeparatedIterator::value_type CommaSeparatedIterator::operator*() const {
    return {first_, second_};
}

/***********************************************************************
 * Conversion to boolean.
 * 
 * Checks if the iterator is still valid (not at the end of the input).
 * 
 * @return True if there are still lines to read, otherwise false.
 **********************************************************************/
CommaSeparatedIterator::operator bool() const {
    return stream_.good() || !first_.empty() || !second_.empty();
}