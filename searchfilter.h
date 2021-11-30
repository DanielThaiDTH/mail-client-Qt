#ifndef SEARCHFILTER_H
#define SEARCHFILTER_H

#include <QString>
#include <string>
#include <QDate>
#include "inbox.h"
#include "DateTime.h"

/**
 * @brief The SearchFilter class is a functor that is used in
 * STL algorithms to find all mails matching the search.
 */
class SearchFilter
{
    std::string query = "";
    std::string to_filter = "";
    std::string from_filter = "";
    std::string subject_filter = "";
    std::string tag_filter = "";
    std::string text_filter = "";
    std::string attach_filter = "";
    QDate start_date;
    QDate end_date;
    bool attachment_check = false;

    std::string toUpper(std::string s) const;
    bool dateBoundCheck(DateTime mail_date) const;

public:
    SearchFilter();
    void setQuery(const QString& newQuery);
    void setToFilter(const QString& filter);
    void setFromFilter(const QString& filter);
    void setSubjectFilter(const QString& filter);
    void setTagFilter(const QString& filter);
    void setTextFilter(const QString& filter);
    void setStartDate(const QDate& newDate);
    void setEndDate(const QDate& newDate);
    void setAttachmentCheck(bool checked);
    void setAttachmentFilter(const QString& filter);

    QString getQuery() const;
    QString getToFilter() const;
    QString getFromFilter() const;
    QString getSubjectFilter() const;
    QString getTagFilter() const;
    QString getTextFilter() const;
    QDate getStartDate() const;
    QDate getEndDate() const;
    bool getAttachmentCheck() const;
    QString getAttachmentFilter() const;

    void reset();
    bool operator()(const Inbox::MailData& data) const;
};

#endif // SEARCHFILTER_H
