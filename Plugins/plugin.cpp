#include "plugin.h"

std::vector<Proposal> Plugin::query(QString txt)
{
    return {};
}
void Plugin::execute(Proposal item) { return; }

Plugin::~Plugin() {}
void Plugin::setup() {}

std::string join(std::string text1, QString txt)
{
    std::string result;
    for(auto c : txt)
    {
        result += text1 + QRegExp::escape(c).toStdString();
    }
    return result;
}
