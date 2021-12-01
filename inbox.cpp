#include "inbox.h"
#include "searchfilter.h"
#include <utility>
#include <algorithm>
#include <cstring>
#include <iostream>

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

    activeBox = BoxType::INBOX;

    sortByDate(inbox_mail);
}

/**
 * @brief Inbox::~Inbox, The destructor handles the end of life of the
 * email objects.
 */
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

    for (MailData& mail_data : junk_mail) {
        if (mail_data.mail != nullptr)
            delete mail_data.mail;
        mail_data.mail = nullptr;
    }

    for (MailData& mail_data : draft_mail) {
        if (mail_data.mail != nullptr)
            delete mail_data.mail;
        mail_data.mail = nullptr;
    }

    for (MailData& mail_data : sent_mail) {
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


QVector<Inbox::MailData>* Inbox::selectBox()
{
    QVector<MailData>* box;

    switch (activeBox) {
    case BoxType::INBOX:
        box = &inbox_mail;
        break;
    case BoxType::TRASH:
        box = &trash_mail;
        break;
    case BoxType::JUNK:
        box = &junk_mail;
        break;
    case BoxType::SENT:
        box = &sent_mail;
        break;
    case BoxType::DRAFT:
        box = &draft_mail;
        break;
    default:
        box = &inbox_mail;
    }

    return box;
}


QVector<MailSummary> Inbox::getInboxSummary()
{
    QVector<MailSummary> inbox_summary;

    QVector<MailData>* box = selectBox();

    for (auto it = box->begin(); it != box->end(); ++it) {
        MailSummary sum;
        sum.setSummary(*it);
        inbox_summary.push_back(sum);
    }

    return inbox_summary;
}

/**
 * @brief Inbox::getMailData, id must be contained in the MailData stored, otherwise
 * behaviour is undefined.
 * @param id
 * @return
 */
const Inbox::MailData& Inbox::getMailData(int id)
{
    QVector<MailData>* box = selectBox();
    auto getByID = [&](MailData& d){ return d.id == id; };

    auto data = std::find_if(box->begin(), box->end(), getByID);

    if (data == box->end())
        return errorMailData;

    data->read = true;
    return *data;
}


const Inbox::MailData& Inbox::getMailDataOffset(int id, int offset)
{
    QVector<MailData>* box = selectBox();
    auto getByID = [&](MailData& d){ return d.id == id; };

    auto data_it = std::find_if(box->begin(), box->end(), getByID);

    if (data_it == box->end())
        return errorMailData;

    if (offset > 0) {
        for (int i = 0; i < offset; i++) {
            ++data_it;
            if (data_it == box->end())
                return errorMailData;
        }
    } else {
        for (int i = 0; i > offset; i--) {
            --data_it;
            if (data_it == box->begin() && i > offset + 1)
                return errorMailData;
        }
    }

    data_it->read = true;
    return *data_it;
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


void Inbox::addEmail(Email *mail)
{
    switch (activeBox) {
    case BoxType::INBOX:
        inbox_mail.push_back(MailData(mail));
        sortByDate(inbox_mail);
        break;
    case BoxType::TRASH:
        trash_mail.push_back(MailData(mail));
        sortByDate(trash_mail);
        break;
    case BoxType::JUNK:
        junk_mail.push_back(MailData(mail));
        sortByDate(junk_mail);
        break;
    case BoxType::SENT:
        sent_mail.push_back(MailData(mail));
        sortByDate(sent_mail);
        break;
    case BoxType::DRAFT:
        draft_mail.push_back(MailData(mail));
        sortByDate(draft_mail);
        break;
    }
}


void Inbox::addEmail(Email *mail, BoxType type)
{
    BoxType originalType = activeBox;
    activeBox = type;
    addEmail(mail);
    activeBox = originalType;
}


std::string Inbox::toUpper(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](char c){ return std::toupper(c); });
    return s;
}


/**
 * @brief Inbox::search, searches on the subject field of the mail. The box
 * searched is the active box.
 * @param query
 * @return QVector<MailSummary>
 */
QVector<MailSummary> Inbox::search(const SearchFilter& filter)
{
//    std::string search_query = query.toStdString();

//    auto search_expr = [&](const MailData& data){
//        auto size = toUpper(data.mail->getSubject()).find(toUpper(search_query));
//        return size != std::string::npos;
//    };

    auto to_summary = [](MailData& data) {
        MailSummary sum;
        sum.setSummary(data);
        return sum;
    };

    QVector<MailData> found;
    QVector<MailData>* box = selectBox();
    QVector<MailSummary> search_results;

    std::copy_if(box->begin(), box->end(), std::back_inserter(found), filter);
    std::transform(found.begin(), found.end(), std::back_inserter(search_results), to_summary);

    return search_results;
}


BoxType Inbox::getActiveBox() const
{
    return activeBox;
}

void Inbox::setActiveBox(BoxType type)
{
    activeBox = type;
}


QSet<QString>& Inbox::getTags()
{
    return tags;
}

QSet<QString>& Inbox::getMailTags(int id)
{
    QVector<MailData>* box = selectBox();
    auto getByID = [&](MailData& d){ return d.id == id; };

    auto data = std::find_if(box->begin(), box->end(), getByID);

    if (data == box->end())
        return errorMailData.tags;

    return data->tags;
}


const QString& Inbox::getLocalAddr() const
{
    return local_addr;
}
