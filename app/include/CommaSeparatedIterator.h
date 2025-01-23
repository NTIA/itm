/** @file CommaSeparatedIterator.h
 * Iterator class for reading comma-delimited input text streams.
 */
#pragma once

#include <istream>  // for std::istream
#include <string>   // For std::string
#include <utility>  // for std::pair

/*******************************************************************************
 * @class CommaSeparatedIterator
 * An iterator that reads lines from an input stream, splitting each line
 * into two strings based on a comma delimiter.
 *
 * This iterator can work with both `std::stringstream` and `std::ifstream`.
 ******************************************************************************/
class CommaSeparatedIterator {
    public:
        /** Type alias for the value returned by the iterator (pair of strings) */
        using value_type = std::pair<std::string, std::string>;

        /***********************************************************************
         * Constructor method
         * 
         * @param[in] stream  The input stream which will be read
         **********************************************************************/
        CommaSeparatedIterator(std::istream &stream);

        /** Pre-increment operator to advance the iterator to the next line */
        CommaSeparatedIterator &operator++();

        /** Dereference operator to obtain the current pair of substrings */
        value_type operator*() const;

        /** Conversion to boolean to check if the iterator is valid */
        explicit operator bool() const;
    private:
        std::istream &stream_; /**< Reference to the input stream */
        std::string line_;     /**< Current line read from the stream */
        std::string first_;    /**< First string from the current line */
        std::string second_;   /**< Second string from the current line */
};
