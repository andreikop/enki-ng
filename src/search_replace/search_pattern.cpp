#include "search_pattern.h"


bool SearchPattern::isEmpty() const {
	return text.isEmpty();
}

bool SearchPattern::isValid() const {
    if (flags & REG_EXP) {
        return QRegularExpression(text).isValid();
    } else {
        return true;
    }
}

QRegularExpression SearchPattern::regExp() const {
    QString patText = text;

    if (flags & WHOLE_WORD) {
        patText = QString("\b" + patText + "\b");
    }

    QRegularExpression::PatternOption qtFlags = QRegularExpression::NoPatternOption;
    if ( ! (flags & CASE_SENSITIVE)) {
        qtFlags = QRegularExpression::CaseInsensitiveOption;
    }

    return QRegularExpression(patText, qtFlags);
}