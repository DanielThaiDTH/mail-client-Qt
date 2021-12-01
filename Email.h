#ifndef EMAIL_H
#define EMAIL_H

#include <string>
#include <vector>
#include "DateTime.h"

/*An email address, split into the local and domain part. A factory function is used to 
create an object of this class.*/
class EmailAddress
{
    std::string local_part;
    std::string domain;
    std::string full;
    EmailAddress(std::string local, std::string domain);
    void setFull(std::string full_addr);
public:
    static EmailAddress* emailAddressFromString(const std::string& mail_string);
    std::string getDomain() const;
    std::string getLocal() const;
    std::string getAddress() const;
};


class Email
{
    std::string subject;
    EmailAddress* to_addr;
    EmailAddress* from_addr;
    DateTime* sendDate;
    DateTime* receiveDate;
    std::string content;
    std::vector<Email> responseTo;
    std::vector<std::string> attachments;
public:
    Email();
    Email(const Email& orig);
    Email(Email&& orig);
    ~Email();
    Email& operator=(const Email& other);
    Email& operator=(Email&& orig);
    int getId() const;
    void setSubject(const std::string& mail_subject);
    std::string getSubject() const;
    void setToAddress(const std::string& to_address);
    std::string getToAddress() const;
    void setFromAddress(const std::string& from_address);
    std::string getFromAddress() const;
    void setSendDate(const std::string& send_date);
    std::string getSendDate() const;
    void setReceiveDate(const std::string& receive_date);
    std::string getReceiveDate() const;
    void setContent(const std::string& content);
    std::string getContent() const;
    void addResponseMail(Email& mail);
    const std::vector<Email> getResponseMails() const;
    void addAttachment(const std::string& filename);
    const std::vector<std::string> getAttachments() const;
    bool operator<(const Email& other) const;
    bool operator>(const Email& other) const;
    bool operator==(const Email& other) const;
    bool operator!=(const Email& other) const;
    bool operator<=(const Email& other) const;
    bool operator>=(const Email& other) const;
};

#endif
