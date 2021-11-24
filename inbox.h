#ifndef INBOX_H
#define INBOX_H

#include <vector>
#include "Email.h"

#include <QObject>
#include <QVector>
#include <QSet>
#include <QMap>
#include <QString>

struct MailSummary;

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
    QVector<MailSummary> getInboxSummary();
    QVector<MailSummary> getTrashSummary();
    const MailData& getMailData(int id) const;
    const MailData& getTrashData(int id) const;
    void moveToTrash(int id);
    void restoreMail(int id);
};


/**
 * @brief Contains summary data for the inbox view items. Stores a bool pointer that
 * refers to a bool in a MailData object, which indicates if a mail is read or not.
 */
struct MailSummary
{
  int id;
  bool* read = nullptr;
  QString subject;
  QString sender;
  QString receiveDate;
  void setSummary(Inbox::MailData& data)
  {
      this->read = &data.read;
      this->id = data.id;
      sender = QString::fromStdString(data.mail->getFromAddress());
      subject = QString::fromStdString(data.mail->getSubject());
      receiveDate = QString::fromStdString(data.mail->getReceiveDate());
  }
};
#endif // INBOX_H
