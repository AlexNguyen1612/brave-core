import * as React from 'react'

import { getLocale } from '$web-common/locale'

import { NavButton, Panel } from '../..'
import {
  ButtonRow,
  SuccessIcon,
  SuccessDescription,
  SuccessText
} from './style'

interface Props {
  headerTitle: string
  description: string
  isPrimaryCTADisabled: boolean
  onClose: () => void
  onClickSecondaryCTA: () => void
  onClickPrimaryCTA: () => void
}

const TransactionComplete = (props: Props) => {
  const {
    headerTitle,
    description,
    isPrimaryCTADisabled,
    onClose,
    onClickPrimaryCTA,
    onClickSecondaryCTA
  } = props

  return (
    <Panel
      navAction={onClose}
      title={headerTitle}
      headerStyle='slim'
    >
      <SuccessIcon />
      <SuccessText>{getLocale('braveWalletTransactionCompleteTitle')}</SuccessText>
      <SuccessDescription>{description}</SuccessDescription>
      <ButtonRow>
        <NavButton
            buttonType='secondary'
            text={getLocale('braveWalletTransactionCompleteReceiptCTA')}
            onSubmit={onClickSecondaryCTA}
        />
        <NavButton
            buttonType='primary'
            text={getLocale('braveWalletTransactionCompleteNextCTA')}
            onSubmit={onClickPrimaryCTA}
            disabled={isPrimaryCTADisabled}
        />
      </ButtonRow>
    </Panel>
  )
}

export default TransactionComplete
