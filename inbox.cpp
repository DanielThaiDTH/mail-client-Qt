#include "inbox.h"
#include <utility>
#include <algorithm>

int Inbox::MailData::id_generator = 0;

/**
 * @brief Inbox::Inbox, Takes a vector of Email pointers and takes control of it. Original vector
 * will now have a list of nullptrs.
 * @param msgs, a vector of Email pointers passed with move semantics
 */
Inbox::Inbox(std::vector<Email*>&& msgs) : QObject()
{
    for (Email* mail : msgs) {
        inbox_mail.push_back(MailData(mail));
        mail_idx.insert(inbox_mail.constLast().id, &(inbox_mail.last()));
    }

    for (Email*& mail_ptr : msgs) {
        mail_ptr = nullptr;
    }

    sortByDate(inbox_mail);
}


Inbox::~Inbox()
{
    for (MailData& mail_data : inbox_mail) {
        if (mail_data.mail != nullptr)
            delete mail_data.mail;
        mail_data.mail = nullptr;
    }

    for (MailData& mail_data : trash_mail) {
        if (mail_data.mail != nullptr)
            delete mail_data.mail;
        mail_data.mail = nullptr;
    }
}


void Inbox::sortByDate(QVector<MailData>& mailbox)
{
    if (mailbox.size() == 0)
        return;

    quicksort(mailbox, 0, (int)mailbox.size() - 1);
}


void Inbox::quicksort(QVector<MailData>& arr, int low, int high)
{
    if (low >= high) {
        return;
    } else if (high - low == 1) {
        if (*(arr[low].mail) < *(arr[high].mail)) {
            std::swap(arr[low], arr[high]);
        } else {
            return;
        }
    }

    int pivot = partition(arr, low, high);
    quicksort(arr, low, pivot - 1);
    quicksort(arr, pivot + 1, high);
}


int Inbox::partition(QVector<MailData> &arr, int low, int high)
{
    if (high - low == 1) {
        if (*(arr[low].mail) < *(arr[high].mail)) {
            std::swap(arr[low], arr[high]);
        } else {
            return high;
        }
    } else if (high <= low) {
        return high;
    }

    //Lomuto parition
    int mid = low + (high - low)/2;
    if (*(arr[mid].mail) > *(arr[low].mail))
        std::swap(arr[mid], arr[low]);
    if (*(arr[high].mail) > *(arr[low].mail))
        std::swap(arr[low], arr[high]);
    if (*(arr[mid].mail) > *(arr[high].mail))
        std::swap(arr[mid], arr[high]);

    int pivot = high;
    int min = low;
    high--;

    while(low < high) {
        while (*(arr[low].mail) > *(arr[pivot].mail)) low++;
        while (*(arr[high].mail) < *(arr[pivot].mail) && high >= min) high--;

        if (low < high) {
            std::swap(arr[low], arr[high]);
            low++;
            high--;
        }
    }

    if (low == high && *(arr[low].mail) > *(arr[pivot].mail))
        low++;

    std::swap(arr[low], arr[pivot]);
    pivot = low;

    return pivot;
}


QVector<MailSummary> Inbox::getInboxSummary() const
{
    QVector<MailSummary> inbox_summary;

    for (auto it = inbox_mail.constBegin(); it != inbox_mail.constEnd(); ++it) {
        MailSummary sum;
        sum.id = it->id;
        sum.read = it->read;
        sum.sender = QString::fromStdString(it->mail->getFromAddress());
        sum.subject = QString::fromStdString(it->mail->getSubject());
        sum.receiveDate = QString::fromStdString(it->mail->getReceiveDate());
        inbox_summary.push_back(sum);
    }

    return inbox_summary;
}


QVector<MailSummary> Inbox::getTrashSummary() const
{
    QVector<MailSummary> trash_summary;

    for (auto it = trash_mail.constBegin(); it != trash_mail.constEnd(); ++it) {
        MailSummary sum;
        sum.id = it->id;
        sum.read = it->read;
        sum.sender = QString::fromStdString(it->mail->getFromAddress());
        sum.subject = QString::fromStdString(it->mail->getSubject());
        sum.receiveDate = QString::fromStdString(it->mail->getReceiveDate());
        trash_summary.push_back(sum);
    }

    return trash_summary;
}


const Inbox::MailData& Inbox::getMailData(int id) const
{
    auto getByID = [&](const MailData& d){ return d.id == id; };
    return *std::find_if(inbox_mail.begin(), inbox_mail.end(), getByID);
}


const Inbox::MailData& Inbox::getTrashData(int id) const
{
    auto getByID = [&](const MailData& d){ return d.id == id; };
    return *std::find_if(trash_mail.begin(), trash_mail.end(), getByID);
}


/**
 * @brief Inbox::moveMail, moves MailData from vector src to dst. The iterator move_it should
 * belong to the src QVector.
 * @param src
 * @param dst
 * @param move_it
 */
void Inbox::moveMail(QVector<MailData>& src, QVector<MailData>& dst, QVector<MailData>::Iterator move_it)
{
    if (&src == &dst)
        return;

    dst.push_back(std::move(*move_it));
    sortByDate(dst);
    src.erase(move_it);
}


void Inbox::moveToTrash(int id)
{
    auto move_it = std::find_if(inbox_mail.begin(), inbox_mail.end(),
                 [&](const MailData& mail) {
                        return mail.id == id;
                    }
    );
    if (move_it != inbox_mail.end())
        moveMail(inbox_mail, trash_mail, move_it);
}



void Inbox::restoreMail(int id)
{
    auto move_it = std::find_if(trash_mail.begin(), trash_mail.end(),
                 [&](const MailData& mail) {
                        return mail.id == id;
                    }
    );
    if (move_it != trash_mail.end())
        moveMail(trash_mail, inbox_mail, move_it);
}



