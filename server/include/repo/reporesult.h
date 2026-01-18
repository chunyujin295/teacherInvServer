/*************************************************
  * 描述：
  *
  * File：reporesult.h
  * Date：2026/1/18
  * Update：
  * ************************************************/
#ifndef TEACHERINVSERVER_REPORESULT_H
#define TEACHERINVSERVER_REPORESULT_H

#pragma once
#include <optional>
#include <domain/apierror.h>

template<class T>
struct RepoResult
{
	bool ok = true;
	std::optional<ApiError> error;
	std::optional<T> value;
};

struct RepoResultVoid
{
	bool ok = true;
	std::optional<ApiError> error;
};


#endif //TEACHERINVSERVER_REPORESULT_H