#ifndef INBOX_H
#define INBOX_H

#include <vector>
#include "Email.h"

#include <QObject>
#include <QVector>
#include <QSet>
#include <QMap>
#include <QString>
#include <QMessageBox>

struct MailSummary;
class SearchFilter;

enum class BoxType { INBOX, TRASH, JUNK, SENT, DRAFT };

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
        MailData()
        {
            id = -1;
            mail = nullptr;
            read = false;
        }
    };

private:
    QVector<MailData> inbox_mail;
    QVector<MailData> trash_mail;
    QVector<MailData> draft_mail;
    QVector<MailData> sent_mail;
    QVector<MailData> junk_mail;
    BoxType activeBox;
    MailData errorMailData;
    QMap<int, MailData*> mail_idx;
    QSet<QString> tags;
    void quicksort(QVector<MailData>& arr, int low, int high);
    int partition(QVector<MailData>& arr, int low, int high);
    void moveMail(QVector<MailData>& src, QVector<MailData>& dst, QVector<MailData>::Iterator move_it);
    std::string toUpper(std::string s);
    QVector<MailData>* selectBox();

public:
    Inbox(std::vector<Email*>&& msgs);
    ~Inbox();
    void sortByDate(QVector<MailData>& vec);
    QVector<MailSummary> getInboxSummary();
    const MailData& getMailData(int id);
    const MailData& getMailDataOffset(int id, int offset);
    void moveToTrash(int id);
    void restoreMail(int id);
    void addEmail(Email* mail);
    void addEmail(Email* mail, BoxType type);
    QVector<MailSummary> search(const SearchFilter& filter);
    BoxType getActiveBox() const;
    void setActiveBox(BoxType type);
    QSet<QString>& getTags();
    QSet<QString>& getMailTags(int id);
};


/**
 * @brief Contains summary data for the inbox view items. Stores a bool pointer that
 * refers to a bool in a MailData object, which indicates if a mail is read or not.
 */
struct MailSummary
{
  int id;
  bool read;
  QString subject;
  QString sender;
  QString receiveDate;
  void setSummary(Inbox::MailData& data)
  {
      this->read = data.read;
      this->id = data.id;
      sender = QString::fromStdString(data.mail->getFromAddress());
      subject = QString::fromStdString(data.mail->getSubject());
      receiveDate = QString::fromStdString(data.mail->getReceiveDate());
  }
};
#endif // INBOX_H
