#include "ProxyListener.hpp"

using namespace antlr4;

ProxyListener::ProxyListener() {}

void ProxyListener::addListener(QueryListener *listener, bool alwaysListen) {
  if (alwaysListen)
    this->listener.push_back(listener);
  else
    this->observers.push_back(listener);
}

void ProxyListener::addAllListener(std::vector<QueryListener *> listenerList,
                                   bool alwaysListen) {
  if (alwaysListen) {
    this->listener.insert(listener.end(), listenerList.begin(),
                          listenerList.end());
  } else {
    this->observers.insert(observers.end(), listenerList.begin(),
                           listenerList.end());
  }
}

bool ProxyListener::removeListener(QueryListener *observer, bool alwaysListen) {
  if (alwaysListen)
    for (auto it = listener.begin(); it != listener.end(); it++) {
      if (*it == observer) {
        observers.erase(it);
        return true;
      }
    }
  else
    for (auto it = observers.begin(); it != observers.end(); it++) {
      if (*it == observer) {
        observers.erase(it);
        return true;
      }
    }
  return false;
}


void ProxyListener::removeAllListener(bool alwaysListen) {
  if (alwaysListen) {
    listener.clear();
  } else {
    observers.clear();
  }
}


void ProxyListener::enterQuery(QueryParser::QueryContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->enterQuery(ctx);
    } catch (netspeak::invalid_query error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->enterQuery(ctx);
    }
  }
}
void ProxyListener::exitQuery(QueryParser::QueryContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->exitQuery(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->exitQuery(ctx);
    }
  }
  if (!errorMessages.empty())
    throw netspeak::invalid_query(errorMessages);
}

void ProxyListener::enterUnits(QueryParser::UnitsContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->enterUnits(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->enterUnits(ctx);
    }
  }
}
void ProxyListener::exitUnits(QueryParser::UnitsContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->exitUnits(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->exitUnits(ctx);
    }
  }
}

void ProxyListener::enterUnit(QueryParser::UnitContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->enterUnit(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->enterUnit(ctx);
    }
  }
}
void ProxyListener::exitUnit(QueryParser::UnitContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->exitUnit(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->exitUnit(ctx);
    }
  }
}

void ProxyListener::enterOptionset(QueryParser::OptionsetContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->enterOptionset(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->enterOptionset(ctx);
    }
  }
}
void ProxyListener::exitOptionset(QueryParser::OptionsetContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->exitOptionset(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->exitOptionset(ctx);
    }
  }
}

void ProxyListener::enterOrderset(QueryParser::OrdersetContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->enterOrderset(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->enterOrderset(ctx);
    }
  }
}
void ProxyListener::exitOrderset(QueryParser::OrdersetContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->exitOrderset(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->exitOrderset(ctx);
    }
  }
}

void ProxyListener::enterDictset(QueryParser::DictsetContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->enterDictset(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->enterDictset(ctx);
    }
  }
}
void ProxyListener::exitDictset(QueryParser::DictsetContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->exitDictset(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->exitDictset(ctx);
    }
  }
}

void ProxyListener::enterPhrase(QueryParser::PhraseContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->enterPhrase(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->enterPhrase(ctx);
    }
  }
}
void ProxyListener::exitPhrase(QueryParser::PhraseContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->exitPhrase(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->exitPhrase(ctx);
    }
  }
}

void ProxyListener::enterAsterisk(QueryParser::AsteriskContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->enterAsterisk(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->enterAsterisk(ctx);
    }
  }
}
void ProxyListener::exitAsterisk(QueryParser::AsteriskContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->exitAsterisk(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->exitAsterisk(ctx);
    }
  }
}

void ProxyListener::enterRegexword(QueryParser::RegexwordContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->enterRegexword(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->enterRegexword(ctx);
    }
  }
}
void ProxyListener::exitRegexword(QueryParser::RegexwordContext *ctx) {
  for (auto observer = observers.begin(); observer != observers.end();
       observer++) {
    (*observer)->exitRegexword(ctx);
  }
}

void ProxyListener::enterWord(QueryParser::WordContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->enterWord(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->enterWord(ctx);
    }
  }
}

void ProxyListener::exitWord(QueryParser::WordContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->exitWord(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->exitWord(ctx);
    }
  }
}

void ProxyListener::enterQmark(QueryParser::QmarkContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->enterQmark(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->enterQmark(ctx);
    }
  }
}
void ProxyListener::exitQmark(QueryParser::QmarkContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->exitQmark(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->exitQmark(ctx);
    }
  }
}

void ProxyListener::enterPlus(QueryParser::PlusContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->enterPlus(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->enterPlus(ctx);
    }
  }
}
void ProxyListener::exitPlus(QueryParser::PlusContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->exitPlus(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->exitPlus(ctx);
    }
  }
}

void ProxyListener::enterAssociation(QueryParser::AssociationContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->enterAssociation(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->enterAssociation(ctx);
    }
  }
}
void ProxyListener::exitAssociation(QueryParser::AssociationContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->exitAssociation(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->exitAssociation(ctx);
    }
  }
}


void ProxyListener::enterEveryRule(antlr4::ParserRuleContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->enterEveryRule(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->enterEveryRule(ctx);
    }
  }
}
void ProxyListener::exitEveryRule(antlr4::ParserRuleContext *ctx) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->exitEveryRule(ctx);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->exitEveryRule(ctx);
    }
  }
}
void ProxyListener::visitTerminal(antlr4::tree::TerminalNode *node) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->visitTerminal(node);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->visitTerminal(node);
    }
  }
}
void ProxyListener::visitErrorNode(antlr4::tree::ErrorNode *node) {
  for (auto observer = listener.begin(); observer != listener.end();
       observer++) {
    try {
      (*observer)->visitErrorNode(node);
    } catch (std::logic_error error) {
      errorMessages.append(error.what());
      errorMessages.append("\n");
    }
  }
  if (valid) {
    for (auto observer = observers.begin(); observer != observers.end();
         observer++) {
      (*observer)->visitErrorNode(node);
    }
  }
}
