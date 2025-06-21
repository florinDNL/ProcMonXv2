#include "pch.h"
#include "CSVEventDataSerializer.h"
#include <fstream>
#include <format>
#include "FormatHelper.h"


//  Needed for processes like msedge which contain commas in their command line, messing up the csv formatting
std::wstring CSVEventDataSerializer::EscapeCommandLine(std::wstring cmd)
{
    std::wstring escapedCmd = L"";
    for (wchar_t c : cmd)
    {
        if (c == '\"')
        {
            escapedCmd += c;
        }
        escapedCmd += c;
    }

    return std::format(L"\"{}\"", escapedCmd);
}

std::wstring CSVEventDataSerializer::SerializeEvent(std::shared_ptr<EventData>& event)
{
    std::wstring ts = (std::wstring)FormatHelper::FormatTime(event->GetTimeStamp());
    std::wstring name = event->GetEventName();
    std::wstring cmd, status, parentId = L"";
    CString img = L"";

    if (name == L"Process/Start" || name == L"Process/End")
    {
        cmd = (std::wstring)(event->GetProperty(L"CommandLine")->GetUnicodeString());
        img = event->GetProperty(L"ImageFileName")->GetAnsiString();
        status = std::to_wstring(event->GetProperty(L"ExitStatus")->GetValue<DWORD>());
        parentId = std::to_wstring(event->GetProperty(L"ParentId")->GetValue<DWORD>());
    }

    std::wstring serial = std::format(L"{},{},{},{},{},{},{},{},{},{}", event->GetIndex(), ts, name, 
                                      event->GetProcessName(), event->GetProcessId(), event->GetThreadId(),
                                      EscapeCommandLine(cmd), (std::wstring)img, status, parentId);
    
    return serial;
}

bool CSVEventDataSerializer::Save(const std::vector<std::shared_ptr<EventData>>& events, const EventDataSerializerOptions& options, PCWSTR path) {
    std::wofstream out;
    out.open(path);
    if(out.fail())
        return false;

    out << L"Id,Time Stamp,Event Name,Process Name,PID,TID,Command Line,Image Name,Exit Status,Parent ID" << std::endl;

    for (std::shared_ptr<EventData> evt : events) {
        out << SerializeEvent(evt) << std::endl;
    }

    out.close();

    return true;
}

std::vector<std::shared_ptr<EventData>> CSVEventDataSerializer::Load(PCWSTR path) {
    return std::vector<std::shared_ptr<EventData>>();
}
