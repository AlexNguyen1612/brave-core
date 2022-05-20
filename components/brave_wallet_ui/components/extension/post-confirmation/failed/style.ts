import styled from 'styled-components'

import ErrorSvg from '../../../../assets/svg-icons/error-circle-icon.svg'

export const ErrorIcon = styled.div`
  width: 112px;
  height: 112px;
  background: url(${ErrorSvg});
  margin: 24px 0;
`

export const ErrorText = styled.span`
  font-family: 'Poppins';
  font-style: normal;
  font-weight: 600;
  font-size: 18px;
  line-height: 27px;

  text-align: center;
  letter-spacing: 0.02em;

  color: ${(p) => p.theme.color.errorBorder};
`

export const ErrorDescription = styled.div`
  font-family: 'Poppins';
  font-style: normal;
  font-weight: 400;
  font-size: 12px;
  line-height: 20px;
  text-align: center;
  color: ${(p) => p.theme.color.text02};

  padding: 8px 16px;
  flex-grow: 1;
`

export const ButtonRow = styled.div`
  display: flex;
  align-items: center;
  justify-content: center;
  flex-direction: row;
  width: 100%;
  padding-bottom: 32px;
`

export const ErrorDetailTitle = styled.div`
  font-family: 'Poppins';
  font-style: normal;
  font-weight: 500;
  font-size: 12px;
  line-height: 18px;
  color: ${p => p.theme.color.errorBorder};
`

export const ErrorDetailContentContainer = styled.div`
  background: ${p => p.theme.color.errorBackground};
  border-radius: 8px;
  margin: 16px;
`

export const ErrorDetailContent = styled.div`
  font-family: 'Poppins';
  font-style: normal;
  font-weight: 400;
  font-size: 12px;
  line-height: 18px;
  color: ${p => p.theme.color.text01};
  opacity: 0.9;
  margin: 8px;
`
