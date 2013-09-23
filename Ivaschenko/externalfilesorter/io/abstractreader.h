#ifndef ABSTRACTREADER_H
#define ABSTRACTREADER_H

#include <iostream>

/**
 * Interface representing abstract fixed-type data reader
 */
template <typename DataType> class AbstractReader
{
	public:
		/**
		 * Default destructor unbinds stream if it's binded
		 */
		virtual ~AbstractReader()
		{
			unbindStream();
		}

		/**
		 * Abstract method that reads one element from the stream
		 * Returns true in case of success, false otherwise
		 */
		virtual bool operator () (DataType &element) = 0;

		/**
		 * Binds stream to the reader
		 */
		virtual void bindStream(std::istream &in)
		{
			stream = &in;
		}

		/**
		 * Unbinds currently binded stream
		 */
		virtual void unbindStream()
		{
			stream = 0;
		}

		/**
		 * Checks if reader is ready to read data
		 */
		virtual bool ready() const
		{
			return stream && *stream;
		}

		/**
		 * Returns a pointer to the binded stream
		 * It can be used for adjusting precision or other options for example
		 */
		virtual std::istream* getStream() const
		{
			return stream;
		}

	protected:
		std::istream *stream;
};

#endif // ABSTRACTREADER_H
