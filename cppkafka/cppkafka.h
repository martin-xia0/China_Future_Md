/*
 * Copyright (c) 2017, Matias Fontanini
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef CPPKAFKA_H
#define CPPKAFKA_H

#include "buffer.h"
#include "clonable_ptr.h"
#include "configuration.h"
#include "configuration_base.h"
#include "configuration_option.h"
#include "consumer.h"
#include "error.h"
#include "event.h"
#include "exceptions.h"
#include "group_information.h"
#include "header.h"
#include "header_list.h"
#include "header_list_iterator.h"
#include "kafka_handle_base.h"
#include "logging.h"
#include "macros.h"
#include "message.h"
#include "message_builder.h"
#include "message_internal.h"
#include "message_timestamp.h"
#include "metadata.h"
#include "producer.h"
#include "queue.h"
#include "topic.h"
#include "topic_configuration.h"
#include "topic_partition.h"
#include "topic_partition_list.h"

#include "utils/backoff_committer.h"
#include "utils/backoff_performer.h"
#include "utils/buffered_producer.h"
#include "utils/compacted_topic_processor.h"
#include "utils/consumer_dispatcher.h"
#include "utils/poll_interface.h"
#include "utils/poll_strategy_base.h"
#include "utils/roundrobin_poll_strategy.h"

#endif
