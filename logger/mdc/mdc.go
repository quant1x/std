package mdc

import (
	"gitee.com/quant1x/pkg/uuid"
	"gitee.com/quant1x/std/logger/gls"
)

const (
	APP_TRACEID = "app-traceid"
)

func init() {
	GenTraceId()
}

func GenTraceId() {
	u1 := uuid.NewV4()
	Set(APP_TRACEID, u1.String())
}

func RemoveTraceId() {
	Remove(APP_TRACEID)
}

func Set(key string, value interface{}) {
	gls.Set(key, value)
}

func Get(key string) interface{} {
	return gls.Get(key)
}

func Remove(key string) {
	gls.Remove(key)
}
