/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef PatternistSDK_TestCase_H
#define PatternistSDK_TestCase_H

#include <QtXmlPatterns/QXmlQuery>

#include "qexternalvariableloader_p.h"

#include "ErrorHandler.h"
#include "TestBaseLine.h"
#include "Global.h"

#include "TestItem.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QDate;
class QString;
class QUrl;

namespace QPatternistSDK
{
    class XMLWriter;

    /**
     * @short A generic abstract base class for test cases.
     *
     * @ingroup PatternistSDK
     * @author Frans Englich <frans.englich@nokia.com>
     */
    class Q_PATTERNISTSDK_EXPORT TestCase : public TestItem
    {
    public:
        /**
         * Corresponds to the simpleType test:scenarios-enum
         */
        enum Scenario
        {
            /**
             * The test case should evaluate normally and that the output
             * should match the supplied base line.
             */
            Standard        = 1,

            /**
             * The test case should result in a static error, a parser error.
             */
            ParseError      = 2,

            /**
             * The test case should result in a dynamic error, a runtime error.
             */
            RuntimeError    = 4,

            Trivial         = 8,

            /**
             * ParseError and RuntimeError OR'd.
             */
            AnyError        = RuntimeError | ParseError

        };

        TestCase();
        virtual ~TestCase();

        /**
         * Executes the test, and returns the result. The returned list
         * will always contain exactly one TestResult.
         *
         * @p stage is ignored when running out-of-process.
         */
        virtual TestResult::List execute(const ExecutionStage stage,
                                         TestSuite *ts);

        /**
         * Determines the corresponding Scenario enumerator from the string
         * representation @p string.
         *
         * The following mappings are in effect:
         * @arg @c Standard "standard"
         * @arg @c ParseError "parse-error"
         * @arg @c RuntimeError "runtime-error"
         */
        static Scenario scenarioFromString(const QString &string);

        /**
         * @return always @c true
         */
        virtual bool isFinalNode() const;

        /**
         * Calling this function makes no sense, so it always
         * performs an Q_ASSERT check.
         */
        virtual void appendChild(TreeItem *);

        /**
         * Calling this function makes no sense, so it always
         * performs an Q_ASSERT check.
         */
        virtual TreeItem *child(const unsigned int) const;

        /**
         * @return always zero
         */
        virtual unsigned int childCount() const;

        /**
         * @return always an empty list.
         */
        virtual TreeItem::List children() const;

        /**
         * A description of the test case for human consumption.
         */
        virtual QString description() const = 0;

        /**
         * The title of the test. This can be the identifier of the test, for example.
         */
        virtual QString title() const = 0;

        /**
         * Whether this test case only make use of XPath features.
         *
         * @returns @c false if the test case exercises any XQuery feature
         * which is not available in XPath 2.0.
         */
        virtual bool isXPath() const = 0;

        /**
         * The full name of the creator of the test case. For example, "Frans Englich".
         */
        virtual QString creator() const = 0;

        /**
         * The date of when the test case was created or last modified.
         */
        virtual QDate lastModified() const = 0;

        /**
         * The test's source code. That is, the XPath/XQuery code for the test.
         *
         * @param ok the function sets this value to @c false if loading the query
         * failed, and returns a description of the error for human consumption. If
         * everything went ok, @p ok is set to @c true, and the query is returned.
         */
        virtual QString sourceCode(bool &ok) const = 0;

        /**
         * The path to the file containing the code of the test case.
         */
        virtual QUrl testCasePath() const = 0;

        /**
         * The test case's identifier. For example, "Literals001".
         */
        virtual QString name() const = 0;

        /**
         * What kind of test this is. For example, whether the test case
         * should result in a parser error or should evaluate without errors.
         *
         * The vast common case is that one Scenario is returned; the bit signifiance
         * is for the TestCase sub-class UserTestCase.
         */
        virtual Scenario scenario() const = 0;

        static QString displayName(const Scenario scen);

        /**
         * @returns the valid test baselines for this test case. If only
         * one outcome is valid, the returned list only contains
         * that baseline.
         */
        virtual TestBaseLine::List baseLines() const = 0;

        virtual TestResult *testResult() const;

        virtual ResultSummary resultSummary() const;

        void toXML(XMLWriter &receiver) const;

        virtual QPatternist::ExternalVariableLoader::Ptr externalVariableLoader() const = 0;

        /**
         * @short The XML document that should be used as focus. If none should
         * be used, and hence the focus be undefined, a default constructed
         * QUrl is returned.
         */
        virtual QUrl contextItemSource() const = 0;

        /**
         * Returns by default QXmlQuery::XQuery10.
         */
        virtual QXmlQuery::QueryLanguage language() const;

        virtual QXmlName initialTemplateName() const;
    private:
        TestResult::List execute(const ExecutionStage stage);
        TestResult *createTestResult(const TestResult::Status status,
                                           const QString &comment) const;

        QPointer<TestResult>    m_result;
    };
}

QT_END_NAMESPACE

QT_END_HEADER

#endif
// vim: et:ts=4:sw=4:sts=4