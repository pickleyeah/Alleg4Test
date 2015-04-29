#include "CSVFile.h"

CSVFile::CSVFile(const char* filename) :
m_entriesLeft(true),
m_fp(NULL)
{
	m_fp = fopen(filename, "r");
	ReadNextLine();
}

CSVFile::~CSVFile()
{
	fclose(m_fp);
}

std::vector<std::string> CSVFile::GetNextEntry()
{
	std::vector<std::string> result;
	if (!m_entriesLeft)
		return result;

	// Separate the string into substrings
	char *substr = strtok(m_buffer, ",");
	while (substr != NULL)
	{
		result.push_back(substr);
		substr = strtok(NULL, ",");
	}

	ReadNextLine();
	return result;
}

void CSVFile::ReadNextLine()
{
	if (!m_entriesLeft)
		return;
	do
	{
		if (fgets(m_buffer, 256, m_fp) == NULL)
		{
			m_entriesLeft = false;
			return;
		}
		// Trim the newline
		int len = strlen(m_buffer);
		if (m_buffer[len - 1] == '\n')
			m_buffer[len - 1] = '\0';
	} while (m_buffer[0] == '#' || m_buffer[0] == '\n' || m_buffer[0] == '\0' || m_buffer[0] == ' ');
}