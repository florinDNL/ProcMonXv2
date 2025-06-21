#pragma once
#include "IEventDataSerializer.h"

class CSVEventDataSerializer : public IEventDataSerializer {
private:
	std::wstring SerializeEvent(std::shared_ptr<EventData>& event);
	std::wstring EscapeCommandLine(std::wstring cmd);
public:
	virtual bool Save(const std::vector<std::shared_ptr<EventData>>& events, const EventDataSerializerOptions& options, PCWSTR path) override;
	virtual std::vector<std::shared_ptr<EventData>> Load(PCWSTR path) override;	
};

