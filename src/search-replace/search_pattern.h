#pragma once

#include <QString>
#include <QRegularExpression>


struct SearchPattern {
	enum SearchFlags {
	    CASE_SENSITIVE = 0x1,
	    REG_EXP = 0x2,
	    WHOLE_WORD = 0x4,
	};

    SearchPattern(const QString& text):
        text(text),
        flags(0)
    {}

    bool isEmpty() const;
    bool isValid() const;
    QRegularExpression regExp() const;

    QString text;
    int flags; // or of SearchFlags
};
