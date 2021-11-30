#include "searchfilter.h"
#include <algorithm>
#include <vector>

SearchFilter::SearchFilter()
{
    start_date = QDate(1970, 1, 1);
    end_date = QDate(2050, 1, 1);
}

std::string SearchFilter::toUpper(std::string s) const
{
    std::transform(s.begin(), s.end(), s.begin(), [](char c){ return std::toupper(c); });
    return s;
}

void SearchFilter::setQuery(const QString& newQuery)
{
    query = newQuery.trimmed().toStdString();
}

void SearchFilter::setToFilter(const QString& filter)
{
    to_filter = filter.trimmed().toStdString();
}
void SearchFilter::setFromFilter(const QString& filter)
{
    from_filter = filter.trimmed().toStdString();
}

void SearchFilter::setSubjectFilter(const QString& filter)
{
    subject_filter = filter.trimmed().toStdString();
}

void SearchFilter::setTagFilter(const QString& filter)
{
    tag_filter = filter.trimmed().toStdString();
}

void SearchFilter::setTextFilter(const QString& filter)
{
    text_filter = filter.trimmed().toStdString();
}

void SearchFilter::setStartDate(const QDate& newDate)
{
    start_date = newDate;
}

void SearchFilter::setEndDate(const QDate& newDate)
{
    end_date = newDate;
}

void SearchFilter::setAttachmentCheck(bool checked)
{
    attachment_check = checked;
}

void SearchFilter::setAttachmentFilter(const QString& filter)
{
    attach_filter = filter.trimmed().toStdString();
}


QString SearchFilter::getQuery() const
{
    return QString::fromStdString(query);
}
QString SearchFilter::getToFilter() const
{
    return QString::fromStdString(to_filter);
}

QString SearchFilter::getFromFilter() const
{
    return QString::fromStdString(from_filter);
}

QString SearchFilter::getSubjectFilter() const
{
    return QString::fromStdString(subject_filter);
}

QString SearchFilter::getTagFilter() const
{
    return QString::fromStdString(tag_filter);
}

QString SearchFilter::getTextFilter() const
{
    return QString::fromStdString(text_filter);
}

QDate SearchFilter::getStartDate() const
{
    return start_date;
}

QDate SearchFilter::getEndDate() const
{
    return end_date;
}

bool SearchFilter::getAttachmentCheck() const
{
    return attachment_check;
}

QString SearchFilter::getAttachmentFilter() const
{
    return QString::fromStdString(attach_filter);
}


void SearchFilter::reset()
{
    query = "";
    to_filter = "";
    from_filter = "";
    subject_filter = "";
    text_filter = "";
    start_date = QDate(1970, 1, 1);
    end_date = QDate(2050, 1, 1);
    attachment_check = false;
    attach_filter = "";
}


bool SearchFilter::dateBoundCheck(DateTime mail_date) const
{
    int mail_day = mail_date.getDay();
    int mail_month = mail_date.getMonth();
    int mail_year = mail_date.getYear();

    int day, month, year;
    end_date.getDate(&year, &month, &day);

    bool to_bound_check = mail_year < year ||
            (mail_year == year && mail_month < month) ||
            (mail_year == year && mail_month == month && mail_day <= day);

    start_date.getDate(&year, &month, &day);
    bool from_bound_check = mail_year > year ||
            (mail_year == year && mail_month > month) ||
            (mail_year == year && mail_month == month && mail_day >= day);

    return to_bound_check && from_bound_check;
}


bool SearchFilter::operator()(const Inbox::MailData& data) const
{
    bool passed = true;
    auto search_expr = [&](const std::string& str, const std::string& q){
        auto size = toUpper(str).find(toUpper(q));
        return size != std::string::npos;
    };

    if (passed && query.size() > 0)
        passed = search_expr(data.mail->getSubject(), query);

    if (passed && to_filter.size() > 0)
        passed = search_expr(data.mail->getToAddress(), to_filter);

    if (passed && from_filter.size() > 0)
        passed = search_expr(data.mail->getFromAddress(), from_filter);

    if (passed && subject_filter.size() > 0)
        passed = search_expr(data.mail->getSubject(), subject_filter);

    if (passed && text_filter.size() > 0)
        passed = search_expr(data.mail->getContent(), text_filter);

    if (passed && tag_filter.size() > 0) {
        if (data.tags.size() > 0) {
            bool found = false;
            for (const QString& tag : data.tags) {
                found = search_expr(tag.toStdString(), tag_filter);
                if (found)
                    break;
            }
            passed = found;
        } else {
            passed = false;
        }
    }

    if (passed)
        passed = dateBoundCheck( DateTime(data.mail->getReceiveDate()) );

    if (passed && attachment_check) {
        std::vector<std::string> attach = data.mail->getAttachments();
        passed = attach.size() > 0;

        if (passed && attach_filter.size() > 0) {
            bool found = false;
            for (const std::string& file : attach) {
                found = search_expr(file, attach_filter);
                if (found)
                    break;
            }

            passed = found;
        }
    }

    return passed;
}
