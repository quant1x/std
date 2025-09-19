package coroutine

import (
	"context"
	"sync"

	"gitee.com/quant1x/std/core"
	"gitee.com/quant1x/std/logger"
)

var (
	globalOnce    sync.Once
	globalContext context.Context    = nil
	globalCancel  context.CancelFunc = nil
)

func initContext() {
	globalContext, globalCancel = context.WithCancel(context.Background())
}

// Context 获取全局顶层context
func Context() context.Context {
	globalOnce.Do(initContext)
	return globalContext
}

// Shutdown 关闭应用程序, 通知所有协程退出
func Shutdown() {
	globalOnce.Do(initContext)
	if globalCancel != nil {
		globalCancel()
	}
}

func GetContextWithCancel() (context.Context, context.CancelFunc) {
	globalOnce.Do(initContext)
	ctx, cancel := context.WithCancel(globalContext)
	return ctx, cancel
}

// WaitForShutdown 阻塞等待关闭信号
func WaitForShutdown() {
	globalOnce.Do(initContext)
	interrupt := core.Notify()
	select {
	case <-globalContext.Done():
		logger.Infof("application shutdown...")
		globalCancel()
		break
	case sig := <-interrupt:
		logger.Infof("interrupt: %s", sig.String())
		globalCancel()
		break
	}
}
