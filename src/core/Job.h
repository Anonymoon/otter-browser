/**************************************************************************
* Otter Browser: Web browser controlled by the user, not vice-versa.
* Copyright (C) 2017 - 2018 Michal Dutkiewicz aka Emdek <michal@emdek.pl>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
**************************************************************************/

#ifndef OTTER_JOB_H
#define OTTER_JOB_H

#include "SearchEnginesManager.h"

#include <QtNetwork/QNetworkReply>

namespace Otter
{

class Job : public QObject
{
	Q_OBJECT

public:
	explicit Job(QObject *parent = nullptr);

public slots:
	virtual void cancel() = 0;

signals:
	void jobFinished(bool isSuccess);
};

class FetchJob : public Job
{
	Q_OBJECT

public:
	explicit FetchJob(const QUrl &url, QObject *parent = nullptr);
	~FetchJob();

	void setTimeout(int seconds);
	void setSizeLimit(qint64 limit);
	QUrl getUrl() const;

public slots:
	void cancel() override;

protected:
	void timerEvent(QTimerEvent *event) override;
	void markAsFailure();
	void markAsFinished();
	virtual void handleSuccessfulReply(QNetworkReply *reply) = 0;

private:
	QNetworkReply *m_reply;
	qint64 m_sizeLimit;
	int m_timeoutTimer;
	bool m_isFinished;
	bool m_isSuccess;
};

class IconFetchJob final : public FetchJob
{
	Q_OBJECT

public:
	explicit IconFetchJob(const QUrl &url, QObject *parent = nullptr);

	QIcon getIcon() const;

protected:
	void handleSuccessfulReply(QNetworkReply *reply) override;

private:
	QIcon m_icon;
};

class SearchEngineFetchJob final : public FetchJob
{
	Q_OBJECT

public:
	explicit SearchEngineFetchJob(const QUrl &url, const QString &identifier = {}, bool saveSearchEngine = true, QObject *parent = nullptr);

	SearchEnginesManager::SearchEngineDefinition getSearchEngine() const;

protected:
	void handleSuccessfulReply(QNetworkReply *reply) override;

private:
	SearchEnginesManager::SearchEngineDefinition m_searchEngine;
	bool m_needsToSaveSearchEngine;
};

}

#endif
