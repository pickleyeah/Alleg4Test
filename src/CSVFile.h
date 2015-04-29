#pragma once

class CSVFile
{
public:
	CSVFile(const char* filename);
	~CSVFile();

	bool HasRemainingEntries() { return m_entriesLeft; }
	std::vector<std::string> GetNextEntry();

private:
	void ReadNextLine();

	FILE *m_fp;
	bool m_entriesLeft;
	char m_buffer[2048];
};