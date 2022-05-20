import * as React from 'react'

import { getLocale } from '$web-common/locale'

import { NavButton, Panel } from '../..'
import {
  ButtonRow,
  ErrorIcon,
  ErrorDescription,
  ErrorText,
  ErrorDetailTitle,
  ErrorDetailContent,
  ErrorDetailContentContainer
} from './style'
import PopupModal from '../../popup-modals'

interface Props {
  headerTitle: string
  isPrimaryCTADisabled: boolean
  errorDetailTitle: string
  errorDetailContent: string
  onClose: () => void
  onClickSecondaryCTA: () => void
  onClickPrimaryCTA: () => void
}

const TransactionFailed = (props: Props) => {
  const {
    headerTitle,
    errorDetailTitle,
    errorDetailContent,
    isPrimaryCTADisabled,
    onClose,
    onClickPrimaryCTA
  } = props

  const [showErrorCodeModal, setShowErrorCodeModal] = React.useState<boolean>(false)

  return (
    <Panel
      navAction={onClose}
      title={headerTitle}
      headerStyle='slim'
    >
      <ErrorIcon />
      <ErrorText>{getLocale('braveWalletTransactionFailedTitle')}</ErrorText>
      <ErrorDescription>
        {getLocale('braveWalletTransactionFailedDescription')}
      </ErrorDescription>
      <ButtonRow>
        <NavButton
            buttonType='secondary'
            text={getLocale('braveWalletTransactionFailedViewErrorCTA')}
            onSubmit={() => setShowErrorCodeModal(true)}
        />
        <NavButton
            buttonType='primary'
            text={getLocale('braveWalletTransactionFailedSwapNextCTA')}
            onSubmit={onClickPrimaryCTA}
            disabled={isPrimaryCTADisabled}
        />
      </ButtonRow>

      {showErrorCodeModal &&
        <ErrorDetail
          title={errorDetailTitle}
          content={errorDetailContent}
          onClose={() => setShowErrorCodeModal(false)}
        />
      }
    </Panel>
  )
}

interface ModalProps {
  title: string
  content: string
  onClose: () => void
}

const ErrorDetail = (props: ModalProps) => {
  const { content, title, onClose } = props

  return (
    <PopupModal
      title={getLocale('braveWalletTransactionFailedModalTitle')}
      onClose={onClose}
    >
      <ErrorDetailTitle>{title}</ErrorDetailTitle>

      <ErrorDetailContentContainer>
        <ErrorDetailContent>{content}</ErrorDetailContent>
      </ErrorDetailContentContainer>

      <ButtonRow>
        <NavButton
          buttonType='primary'
          text={getLocale('braveWalletTransactionFailedModalClose')}
          onSubmit={onClose}
        />
      </ButtonRow>
    </PopupModal>
  )
}

export default TransactionFailed
