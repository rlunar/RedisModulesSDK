#include "../redismodule.h"
#include "../rmutil/util.h"
#include "../rmutil/strings.h"
#include "../rmutil/test_util.h"

/*
 * ====================================================================================================================
 * I M P L E M E N T I N G   Z P O P
 * ====================================================================================================================
 */

int ZPop(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
  if (argc != 2) {
    RedisModule_WrongArity(ctx);
    return REDISMODULE_OK;
  }

  RedisModule_AutoMemory(ctx);
  RedisModuleCallReply *rep = RedisModule_Call(ctx, "ZRANGE", "!sllc", argv[1], 0, 0, "WITHSCORES");

  if (RedisModule_CallReplyType(rep) == REDISMODULE_REPLY_ERROR) {
    RedisModule_ReplyWithCallReply(ctx, rep);
    return REDISMODULE_OK;
  }

  RedisModuleString *ele =
      RedisModule_CreateStringFromCallReply(RedisModule_CallReplyArrayElement(arr, 0));
  RedisModule_Call(ctx, "ZREM", "ss", key, ele);
  RedisModule_ReplyWithCallReply(ctx, rep);

  return REDISMODULE_OK;
}

int RedisModule_OnLoad(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
  if (RedisModule_Init(ctx, "zpop", 1, REDISMODULE_APIVER_1) == REDISMODULE_ERR) {
    return REDISMODULE_ERR;
  }

  if (RedisModule_CreateCommand(ctx, "z.pop", "Zpop", "Write", 1, 1, 1) == REDISMODULE_ERR) {
    return REDISMODULE_ERR;
  }

  return REDISMODULE_OK;
}