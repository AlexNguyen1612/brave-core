/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import { loadTimeData } from '../../../../common/loadTimeData'

export const LEDGER_BRIDGE_URL = loadTimeData.getString('braveWalletLedgerBridgeUrl').slice(0, -1) // Strip off trailing '/' in URL

export enum LedgerCommand {
  Unlock = 'ledger-unlock',
  GetAccount = 'ledger-get-accounts',
  SignTransaction = 'ledger-sign-transaction',
  AuthorizationRequired = 'authorization-required', // Sent by the frame to the parent context
  AuthorizationSuccess = 'authorization-success' // Sent by the frame to the parent context
}

export enum LedgerErrorsCodes {
  BridgeNotReady = 0,
  CommandInProgress = 1
}

export type CommandMessage = {
  command: LedgerCommand
  id: string
  origin: string
}

export type LedgerResponsePayload = {
  success: boolean
}

export type LedgerError = LedgerResponsePayload & {
  message?: string
  statusCode?: number
  id?: string
  name?: string
}

// Unlock command
// Note: Relative to the Trezor messages, *Response and *ResponsePayload
// are reversed. The Ledger *Response messages will have a payload field
// of type *ResponsePayload, whereas Trezor will have a *ResponsePayload
// messages with a payload field of type *Response.
export type UnlockResponse= CommandMessage & {
  payload: LedgerResponsePayload | LedgerError
}
export type UnlockCommand = CommandMessage & {
  command: LedgerCommand.Unlock
}

// GetAccounts command
export type GetAccountResponsePayload = LedgerResponsePayload & {
  address: Buffer
}
export type GetAccountResponse = CommandMessage & {
  payload: GetAccountResponsePayload | LedgerError
}
export type GetAccountCommand = CommandMessage & {
  command: LedgerCommand.GetAccount
  path: string
}

// SignTransaction command
export type SignTransactionResponsePayload = LedgerResponsePayload & {
  signature: Buffer
}
export type SignTransactionResponse= CommandMessage & {
  payload: SignTransactionResponsePayload | LedgerError
}
export type SignTransactionCommand = CommandMessage & {
  command: LedgerCommand.SignTransaction
  path: string
  rawTxBytes: Buffer
}

// AuthorizationRequired command
export type AuthorizationRequiredCommand = CommandMessage & {
  command: LedgerCommand.AuthorizationRequired
}
export type AuthorizationSuccessResponsePayload = CommandMessage & {
  payload: LedgerResponsePayload
}
export type AuthorizationSuccessCommand = CommandMessage & {
  command: LedgerCommand.AuthorizationSuccess
}

export type LedgerFrameCommand = UnlockCommand | GetAccountCommand | SignTransactionCommand | AuthorizationRequiredCommand | AuthorizationSuccessCommand
export type LedgerFrameResponse = UnlockResponse| GetAccountResponse| SignTransactionResponse

type LedgerCommandHandler <T>= ((command: LedgerFrameCommand) => Promise<T>)
type LedgerCommandResponseHandler <T>= ((response: T) => void)
export type LedgerCommandHandlerUnion <T>= LedgerCommandHandler<T> | LedgerCommandResponseHandler<T>
