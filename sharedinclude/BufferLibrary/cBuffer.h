#pragma once
#include <cstdint>
#include <vector>
#include <string>

class cBuffer
{
public:

	cBuffer(uint8_t size);

	void WriteShort(short value);
	void WriteShort(short value, size_t index);

	void WriteInt(int value);
	void WriteInt(int value, size_t index);

	void WriteString(std::string message);

	int ReadInt();
	short ReadShort();
	std::string ReadString(int length);

	std::string GetBufferMessage();
	void SetBufferMessage(std::string message);

	void ClearBuffer();
	int BufferSize();

private:
	std::vector<uint8_t> _buffer;
	size_t writeIndex;
	size_t readIndex;
};