#include "Email.h"
#include "DateTime.h"

#include <utility>

/***Email Address***/


EmailAddress::EmailAddress(std::string local, std::string domain)
{
    local_part = local;
    this->domain = domain;
}


void EmailAddress::setFull(std::string full_addr)
{
    full = full_addr;
}


EmailAddress* EmailAddress::emailAddressFromString(const std::string& mail_string)
{
    size_t at_pos = mail_string.find('@');

    if (at_pos == std::string::npos) {
        EmailAddress* addr = new EmailAddress("", "");
        addr->setFull(mail_string);
        return addr;
    }

    EmailAddress* addr = new EmailAddress(mail_string.substr(0, at_pos), mail_string.substr(at_pos+1));
    addr->setFull(mail_string);
    return addr;
}


std::string EmailAddress::getDomain() const
{
    return domain;
}

std::string EmailAddress::getLocal() const
{
    return local_part;
}

std::string EmailAddress::getAddress() const
{
    return full;
}
/***Email Address***/


/***Email***/

Email::Email()
{
    to_addr = nullptr;
    from_addr = nullptr;
    sendDate = nullptr;
    receiveDate = nullptr;
}


Email::Email(const Email& orig) : Email()
{
    this->subject = orig.subject;
    this->sendDate = orig.sendDate;
    this->receiveDate = orig.receiveDate;
    this->content = orig.content;

    if (orig.to_addr != nullptr)
        this->to_addr = new EmailAddress(*(orig.to_addr));

    if (orig.from_addr != nullptr)
        this->from_addr = new EmailAddress(*(orig.from_addr));

    if (orig.sendDate != nullptr)
        this->sendDate = new DateTime(*(orig.sendDate));

    if (orig.receiveDate != nullptr)
        this->receiveDate = new DateTime(*(orig.receiveDate));
}


Email::Email(Email&& orig) : Email()
{
    this->subject = orig.subject;
    this->sendDate = orig.sendDate;
    this->receiveDate = orig.receiveDate;
    this->content = orig.content;

    this->to_addr = orig.to_addr;
    this->from_addr = orig.from_addr;
    orig.to_addr = nullptr;
    orig.from_addr = nullptr;

    this->sendDate = orig.sendDate;
    this->receiveDate = orig.receiveDate;
    orig.receiveDate = nullptr;
    orig.sendDate = nullptr;
}


Email::~Email()
{
    if (to_addr != nullptr)
        delete to_addr;
    if (from_addr != nullptr)
        delete from_addr;
    if (sendDate != nullptr)
        delete sendDate;
    if (receiveDate != nullptr)
        delete receiveDate;
}


Email& Email::operator=(const Email& orig)
{
    this->subject = orig.subject;
    this->sendDate = orig.sendDate;
    this->receiveDate = orig.receiveDate;
    this->content = orig.content;

    delete this->to_addr;
    delete this->from_addr;

    this->to_addr = new EmailAddress(*(orig.to_addr));
    this->from_addr = new EmailAddress(*(orig.from_addr));
    this->sendDate = new DateTime(*(orig.sendDate));
    this->receiveDate = new DateTime(*(orig.receiveDate));

    return *this;
}


Email& Email::operator=(Email&& orig)
{
    this->subject = orig.subject;
    this->sendDate = orig.sendDate;
    this->receiveDate = orig.receiveDate;
    this->content = orig.content;

    delete this->to_addr;
    delete this->from_addr;

    this->to_addr = orig.to_addr;
    this->from_addr = orig.from_addr;
    orig.to_addr = nullptr;
    orig.from_addr = nullptr;

    delete this->sendDate;
    delete this->receiveDate;

    this->sendDate = orig.sendDate;
    this->receiveDate = orig.receiveDate;
    orig.receiveDate = nullptr;
    orig.sendDate = nullptr;

    return *this;
}


int Email::getId() const
{
    return 0;
}


void Email::setSubject(const std::string& mail_subject)
{
    subject = mail_subject;
}

std::string Email::getSubject() const
{
    return subject;
}


void Email::setToAddress(const std::string& to_address)
{
    if (to_addr != nullptr)
        delete to_addr;

    this->to_addr = EmailAddress::emailAddressFromString(to_address);
}

std::string Email::getToAddress() const
{
    return to_addr->getAddress();
}


void Email::setFromAddress(const std::string& from_address)
{
    if (from_addr != nullptr)
        delete from_addr;

    this->from_addr = EmailAddress::emailAddressFromString(from_address);
}

std::string Email::getFromAddress() const
{
    return from_addr->getAddress();
}


void Email::setSendDate(const std::string& send_date)
{
    if (sendDate != nullptr)
        delete sendDate;

    sendDate = new DateTime(send_date);
}

std::string Email::getSendDate() const
{
    return sendDate->getDateTimeString();
}


void Email::setReceiveDate(const std::string& receive_date)
{
    if (receiveDate != nullptr)
        delete receiveDate;

    receiveDate = new DateTime(receive_date);
}

std::string Email::getReceiveDate() const
{
    if (receiveDate)
        return receiveDate->getDateTimeString();
    else
        return sendDate ? sendDate->getDateTimeString() : "";
}


void Email::setContent(const std::string& content)
{
    this->content = content;
}

std::string Email::getContent() const
{
    return content;
}


void Email::addResponseMail(Email& mail)
{
    responseTo.push_back(std::move(mail));
}

const std::vector<Email> Email::getResponseMails() const
{
    return responseTo;
}


void Email::addAttachment(const std::string& filename)
{
    attachments.push_back(filename);
}

const std::vector<std::string> Email::getAttachments() const
{
    return attachments;
}


bool Email::operator<(const Email& other) const
{
    const DateTime* date = this->receiveDate;
    if (receiveDate->getYear() < other.receiveDate->getYear()) {
        return true;
    } else if (date->getYear() == other.receiveDate->getYear() && date->getMonth() < other.receiveDate->getMonth()) {
        return true;
    } else if (date->getYear() == other.receiveDate->getYear() && date->getMonth() == other.receiveDate->getMonth()
               && date->getDay() < other.receiveDate->getDay()) {
        return true;
    } else if (date->getYear() == other.receiveDate->getYear() && date->getMonth() == other.receiveDate->getMonth()
               && date->getDay() == other.receiveDate->getDay() && date->getHour() < other.receiveDate->getHour()) {
        return true;
    } else if (date->getYear() == other.receiveDate->getYear() && date->getMonth() == other.receiveDate->getMonth()
               && date->getDay() == other.receiveDate->getDay() && date->getHour() == other.receiveDate->getHour()
               && date->getMinute() < other.receiveDate->getMinute()) {
        return true;
    } else {
        return false;
    }
}


bool Email::operator>(const Email &other) const
{
    return other < *this && !(other == *this);
}


bool Email::operator==(const Email &other) const
{
    const DateTime* date = this->receiveDate;

    return date->getYear() == other.receiveDate->getYear() && date->getMonth() == other.receiveDate->getMonth()
            && date->getDay() == other.receiveDate->getDay() && date->getHour() == other.receiveDate->getHour()
            && date->getMinute() == other.receiveDate->getMinute();
}


bool Email::operator<=(const Email &other) const
{
    return *this < other || *this == other;
}


bool Email::operator>=(const Email &other) const
{
    return *this > other || *this == other;
}


bool Email::operator!=(const Email& other) const
{
    return !(*this == other);
}
/***Email***/
