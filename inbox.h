#ifndef INBOX_H
#define INBOX_H

#include <vector>
#include "Email.h"

#include <QObject>
#include <QVector>
#include <QSet>
#include <QMap>
#include <QString>

struct MailSummary
{
  int id;
  bool read;
  QString subject;
  QString sender;
  QString receiveDate;
};

class Inbox : public QObject
{
    Q_OBJECT

public:
    struct MailData
    {
        static int id_generator;
        int id;
        Email* mail;
        bool read;
        QSet<QString> tags;
        MailData(Email* mail)
        {
            id = id_generator++;
            this->mail = mail;
            read = false;
        }
    };

private:
    QVector<MailData> inbox_mail;
    QVector<MailData> trash_mail;
    QVector<MailData> search_mail;
    QMap<int, MailData*> mail_idx;
    void quicksort(QVector<MailData>& arr, int low, int high);
    int partition(QVector<MailData>& arr, int low, int high);
    void moveMail(QVector<MailData>& src, QVector<MailData>& dst, QVector<MailData>::Iterator move_it);

public:
    Inbox(std::vector<Email*>&& msgs);
    ~Inbox();
    void sortByDate(QVector<MailData>& vec);
    QVector<MailSummary> getInboxSummary() const;
    QVector<MailSummary> getTrashSummary() const;
    const MailData& getMailData(int id) const;
    const MailData& getTrashData(int id) const;
    void moveToTrash(int id);
    void restoreMail(int id);
};

#endif // INBOX_H
