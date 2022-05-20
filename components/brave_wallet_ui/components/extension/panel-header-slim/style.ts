import styled from 'styled-components'
import CloseIcon from '../../../assets/svg-icons/close.svg'
import { WalletButton } from '../../shared/style'

export const HeaderTitle = styled.span`
  font-family: Poppins;
  font-size: 12px;
  line-height: 18px;
  font-weight: 500;
  letter-spacing: 0.01em;
  color: ${p => p.theme.color.text03};
  margin-left: auto;
`

export const HeaderWrapper = styled.div`
  display: flex;
  width: 100%;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  border-bottom: ${(p) => `1px solid ${p.theme.color.divider01}`};
  padding: 0 12px;
  margin-bottom: 8px;
`

export const TopRow = styled.div`
  display: flex;
  height: 47px;
  width: 100%;
  flex-direction: row;
  align-items: center;
  justify-content: center;
`

export const CloseButton = styled(WalletButton)`
  margin-left: auto;
  cursor: pointer;
  width: 16px;
  height: 16px;
  background: url(${CloseIcon});
  outline: none;
  border: none;
`
