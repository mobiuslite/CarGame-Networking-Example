#include "cBuffer.h"

cBuffer::cBuffer(uint8_t size)
{
	_buffer.resize(size);
	ClearBuffer();
}

void cBuffer::WriteShort(short value)
{
	if (_buffer.size() < writeIndex + 2)
	{
		_buffer.resize((size_t)writeIndex + 2);
	}

	readIndex = 0;
	_buffer[writeIndex + 1] = (uint8_t)value;
	_buffer[writeIndex] = value >> 8;
	writeIndex += 2;
}

//Writes a short at a certain index.
void cBuffer::WriteShort(short value, size_t index)
{
	if (_buffer.size() < index + 2)
	{
		_buffer.resize((size_t)writeIndex + 2);
	}

	readIndex = 0;
	_buffer[index + 1] = (uint8_t)value;
	_buffer[index] = value >> 8;
}
void cBuffer::WriteInt(int value)
{
	if (_buffer.size() < writeIndex + 4)
	{
		_buffer.resize((size_t)writeIndex + 4);
	}

	readIndex = 0;
	_buffer[writeIndex + 3] = value;
	_buffer[writeIndex + 2] = value >> 8;
	_buffer[writeIndex + 1] = value >> 16;
	_buffer[writeIndex] = value >> 24;
	writeIndex += 4;
}

//Writes a int at a certain index.
void cBuffer::WriteInt(int value, size_t index)
{
	if (_buffer.size() < index + 4)
	{
		_buffer.resize((size_t)writeIndex + 4);
	}

	readIndex = 0;
	_buffer[index + 3] = value;
	_buffer[index + 2] = value >> 8;
	_buffer[index + 1] = value >> 16;
	_buffer[index] = value >> 24;
}

void cBuffer::WriteString(std::string message)
{
	if (_buffer.size() < writeIndex + (int)message.size())
	{
		_buffer.resize((size_t)writeIndex + message.size());
	}

	readIndex = 0;
	for (char c : message)
	{
		_buffer[writeIndex++] = c;
	}
}

int cBuffer::ReadInt()
{
	writeIndex = 0;
	int value = _buffer[readIndex + 3];
	value |= _buffer[readIndex + 2] << 8;
	value |= _buffer[readIndex + 1] << 16;
	value |= _buffer[readIndex] << 24;
	readIndex += 4;

	return value;
}
short cBuffer::ReadShort()
{
	writeIndex = 0;
	short value = _buffer[readIndex + 1];
	value |= _buffer[readIndex] << 8;
	readIndex += 2;

	return value;
}
std::string cBuffer::ReadString(int length)
{
	writeIndex = 0;
	std::string message;

	for (int i = 0; i < length; i++)
	{
		message.push_back(_buffer[readIndex++]);
	}

	return message;
}

std::string cBuffer::GetBufferMessage()
{
	std::string returnString;

	for(int i = 0; i < writeIndex; i++)
	{
		returnString.push_back(_buffer[i]);
	}

	return returnString;
}

void cBuffer::SetBufferMessage(std::string message)
{
	if (_buffer.size() < message.size())
	{
		_buffer.resize(message.size());
	}

	ClearBuffer();
	for (int i = 0; i < (int)message.size(); i++)
	{
		_buffer[i] = message[i];
	}
}

void cBuffer::ClearBuffer()
{
	readIndex = 0;
	writeIndex = 0;
	std::fill(_buffer.begin(), _buffer.end(), 0);
}
int cBuffer::BufferSize()
{
	return (int)_buffer.size();
}